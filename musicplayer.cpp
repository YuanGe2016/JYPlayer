/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "musicplayer.h"
#include "volumebutton.h"

#include <QtWidgets>
#include <QtWinExtras>

MusicPlayer::MusicPlayer(QWidget *parent) : QWidget(parent)
{
    createWidgets();
    createShortcuts();
    createJumpList();
    createTaskbar();
    createThumbnailToolBar();

    connect(&mediaPlayer, &QMediaPlayer::positionChanged, this, &MusicPlayer::updatePosition);
    connect(&mediaPlayer, &QMediaPlayer::durationChanged, this, &MusicPlayer::updateDuration);
    connect(&mediaPlayer, &QMediaObject::metaDataAvailableChanged, this, &MusicPlayer::updateInfo);

    typedef void(QMediaPlayer::*ErrorSignal)(QMediaPlayer::Error);
    connect(&mediaPlayer, static_cast<ErrorSignal>(&QMediaPlayer::error),
            this, &MusicPlayer::handleError);
    connect(&mediaPlayer, &QMediaPlayer::stateChanged,
            this, &MusicPlayer::updateState);

    stylize();
    setAcceptDrops(true);

    happyplayer = new QMediaPlayer;
    happyplayer->setMedia(QUrl::fromLocalFile("./Happybirthday.mp3"));

    rose = new Rose();
    rose->hide();
}

QStringList MusicPlayer::supportedMimeTypes()
{
    QStringList result = QMediaPlayer::supportedMimeTypes();
    if (result.isEmpty())
        result.append(QStringLiteral("audio/mpeg"));
    return result;
}

void MusicPlayer::openFile()
{
    QFileDialog *fileDialog = new QFileDialog(this, "Select Dir", "file");
    fileDialog->setFileMode(QFileDialog::Directory);
    //fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    //fileDialog.setWindowTitle(tr("Open File"));
    fileDialog->setMimeTypeFilters(MusicPlayer::supportedMimeTypes());
    fileDialog->setDirectory(QStandardPaths::standardLocations(QStandardPaths::HomeLocation).value(0, QDir::homePath()));

    if (fileDialog->exec() == QDialog::Accepted) {
        qDebug() << fileDialog->directory().path();
        QDir *dir = new QDir(fileDialog->directory());
        QStringList filter;
        filter << "*.mp3" << "*.mp4" << "*.qmc3";
        dir->setNameFilters(filter);
        QList<QFileInfo> fileInfoList = QList <QFileInfo>(dir->entryInfoList(filter));

        for(int i = 0 ; i < fileInfoList.size(); i++){
            QUrl url("file:///" + fileInfoList.at(i).filePath());
            songUrlMap.insert(fileInfoList.at(i).fileName(),url);
        }

        QStringList tempList = songUrlMap.keys();
        qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
        while(!tempList.isEmpty()) {
            int num = qrand() % tempList.size();
            songList.push_back(tempList.at(num));
            tempList.removeAt(num);
        }
        tempList.clear();
        setSongListWidget();
        playUrl(songUrlMap.value(songList.at(0)));
        currentUrlNum = 0;
        setPlayingColor(currentUrlNum, "yellow");
    }

}

void MusicPlayer::playUrl(const QUrl &url)
{
    playButton->setEnabled(true);
    skipForwardButton->setEnabled(true);
    skipBackButton->setEnabled(true);
    if (url.isLocalFile()) {
        const QString filePath = url.toLocalFile();
        infoLabel->setText(QDir::toNativeSeparators(filePath));
        fileName = QFileInfo(filePath).fileName();
        setTitleRoll();
    } else {
        titleRollTimer.stop();
        setWindowTitle("MZ音乐");
        infoLabel->setText(url.toString());
        fileName.clear();
    }
    mediaPlayer.setMedia(url);
    mediaPlayer.play();
}

void MusicPlayer::playNextSong()
{
    setPlayingColor(currentUrlNum, "white");
    currentUrlNum = (currentUrlNum + 1) % songList.size();
    playUrl(songUrlMap.value( songList.at( currentUrlNum)));
    setPlayingColor(currentUrlNum, "yellow");
}

void MusicPlayer::playLastSong()
{
    setPlayingColor(currentUrlNum, "white");
    currentUrlNum = (currentUrlNum - 1) % songList.size();
    playUrl(songUrlMap.value(songList.at(currentUrlNum)));
    setPlayingColor(currentUrlNum, "yellow");
}

void MusicPlayer::playSelectedSong(QListWidgetItem *item)
{
    setPlayingColor(currentUrlNum, "white");
    QString song = item->text();
    currentUrlNum = songList.indexOf(song);
    playUrl(songUrlMap.value(song));
    setPlayingColor(currentUrlNum, "yellow");
}

void MusicPlayer::setTitleRoll()
{

    QString titleContent = "MZ音乐 - " + fileName + "    ";
    static int nPos = 0;
    static int titleLength = titleContent.length();
    connect(&titleRollTimer, &QTimer::timeout, [=](){

        if (nPos > titleLength)
            nPos = 0;
        setWindowTitle(titleContent.mid(nPos) + titleContent.left(nPos));
        nPos++;
    });
    titleRollTimer.start(400);
}

void MusicPlayer::setPlayingColor(int row, QString color)
{
    songListWidget->item(row)->setTextColor(QColor(color));
}

void MusicPlayer::togglePlayback()
{
    if (mediaPlayer.mediaStatus() == QMediaPlayer::NoMedia)
        openFile();
    else if (mediaPlayer.state() == QMediaPlayer::PlayingState)
        mediaPlayer.pause();
    else
        mediaPlayer.play();
}

void MusicPlayer::seekForward()
{
    positionSlider->triggerAction(QSlider::SliderPageStepAdd);
}

void MusicPlayer::seekBackward()
{
    positionSlider->triggerAction(QSlider::SliderPageStepSub);
}

//! [0]
bool MusicPlayer::event(QEvent *event)
{
    if (event->type() == QWinEvent::CompositionChange || event->type() == QWinEvent::ColorizationChange)
        stylize();
    return QWidget::event(event);
}
//! [0]

static bool canHandleDrop(const QDropEvent *event)
{
    const QList<QUrl> urls = event->mimeData()->urls();
    if (urls.size() != 1)
        return false;
    QMimeDatabase mimeDatabase;
    return MusicPlayer::supportedMimeTypes().
        contains(mimeDatabase.mimeTypeForUrl(urls.constFirst()).name());
}

void MusicPlayer::dragEnterEvent(QDragEnterEvent *event)
{
    event->setAccepted(canHandleDrop(event));
}

void MusicPlayer::dropEvent(QDropEvent *event)
{
    event->accept();
    playUrl(event->mimeData()->urls().constFirst());
}

void MusicPlayer::mousePressEvent(QMouseEvent *event)
{
    offset = event->globalPos() - pos();
    event->accept();
}

void MusicPlayer::mouseMoveEvent(QMouseEvent *event)
{
    move(event->globalPos() - offset);
    event->accept();
}

void MusicPlayer::mouseReleaseEvent(QMouseEvent *event)
{
    offset = QPoint();
    event->accept();
}

//! [1]
void MusicPlayer::stylize()
{   
    if (QtWin::isCompositionEnabled()) {
        QtWin::extendFrameIntoClientArea(this, -1, -1, -1, -1);
        setAttribute(Qt::WA_TranslucentBackground, true);
        setAttribute(Qt::WA_NoSystemBackground, false);
        setStyleSheet(QStringLiteral("MusicPlayer { background: transparent; }"));
    } else {
        QtWin::resetExtendedFrame(this);
        setAttribute(Qt::WA_TranslucentBackground, false);
        setStyleSheet(QStringLiteral("MusicPlayer { background: %1; }").arg(QtWin::realColorizationColor().name()));
    }
    volumeButton->stylize();
    songListWidget->verticalScrollBar()->setStyleSheet(
                "QScrollBar:vertical {margin: 0px 0px 0px 0px;background-color: rgb(50, 50, 50); border: 0px; width: 8px;}"
                "QScrollBar::add-page:vertical{background-color:grey;}"
                "QScrollBar::sub-page:vertical{background-color:grey;}"
                );
    songListWidget->horizontalScrollBar()->setStyleSheet(
                "QScrollBar:horizontal {margin: 0px 0px 0px 0px;background-color: rgb(50, 50, 50); border: 0px; width: 8px;}"
                "QScrollBar::add-page:vertical{background-color:grey;}"
                "QScrollBar::sub-page:vertical{background-color:grey;}"
                );
    songListWidget->setStyleSheet("QListWidget{background-color:black;color:white;}");

}
//! [1]

void MusicPlayer::updateState(QMediaPlayer::State state)
{
    if (state == QMediaPlayer::PlayingState) {
        playButton->setToolTip(tr("Pause"));
        playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    } else {
        playButton->setToolTip(tr("Play"));
        playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }
}

static QString formatTime(qint64 timeMilliSeconds)
{
    qint64 seconds = timeMilliSeconds / 1000;
    const qint64 minutes = seconds / 60;
    seconds -= minutes * 60;
    return QStringLiteral("%1:%2")
        .arg(minutes, 2, 10, QLatin1Char('0'))
        .arg(seconds, 2, 10, QLatin1Char('0'));
}

void MusicPlayer::updatePosition(qint64 position)
{
    positionSlider->setValue(position);
    positionLabel->setText(formatTime(position));
}

void MusicPlayer::updateDuration(qint64 duration)
{
    positionSlider->setRange(0, duration);
    positionSlider->setEnabled(duration > 0);
    positionSlider->setPageStep(duration / 10);
    updateInfo();
}

void MusicPlayer::setPosition(int position)
{
    // avoid seeking when the slider value change is triggered from updatePosition()
    if (qAbs(mediaPlayer.position() - position) > 99)
        mediaPlayer.setPosition(position);
    if(position >= positionSlider->maximum())
        playNextSong();
}

void MusicPlayer::setSongListWidget()
{
    songListWidget->clear();
    songListWidget->addItems(songList);
}

void MusicPlayer::updateInfo()
{
    QStringList info;
    if (!fileName.isEmpty())
        info.append(fileName);
    info.append(formatTime(mediaPlayer.duration()));
    infoLabel->setText(info.join(tr(" - ")));
}

void MusicPlayer::handleError()
{
    playButton->setEnabled(false);
    const QString errorString = mediaPlayer.errorString();
    infoLabel->setText(errorString.isEmpty()
                       ? tr("Unknown error #%1").arg(int(mediaPlayer.error()))
                       : tr("Error: %1").arg(errorString));
}

//! [2]
void MusicPlayer::updateTaskbar()
{
    switch (mediaPlayer.state()) {
    case QMediaPlayer::PlayingState:
        taskbarButton->setOverlayIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        taskbarProgress->show();
        taskbarProgress->resume();
        break;
    case QMediaPlayer::PausedState:
        taskbarButton->setOverlayIcon(style()->standardIcon(QStyle::SP_MediaPause));
        taskbarProgress->show();
        taskbarProgress->pause();
        break;
    case QMediaPlayer::StoppedState:
        taskbarButton->setOverlayIcon(style()->standardIcon(QStyle::SP_MediaStop));
        taskbarProgress->hide();
        break;
    }
}
//! [2]

//! [3]
void MusicPlayer::updateThumbnailToolBar()
{
    playToolButton->setEnabled(mediaPlayer.duration() > 0);
    backwardToolButton->setEnabled(mediaPlayer.position() > 0);
    forwardToolButton->setEnabled(mediaPlayer.position() < mediaPlayer.duration());

    if (mediaPlayer.state() == QMediaPlayer::PlayingState) {
        playToolButton->setToolTip(tr("Pause"));
        playToolButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    } else {
        playToolButton->setToolTip(tr("Play"));
        playToolButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }
}
//! [3]

void MusicPlayer::createWidgets()
{
    playButton = new QToolButton(this);
    playButton->setEnabled(false);
    playButton->setToolTip(tr("Play"));
    playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    connect(playButton, &QAbstractButton::clicked, this, &MusicPlayer::togglePlayback);

    skipForwardButton = new QToolButton(this);
    skipForwardButton->setEnabled(false);
    skipForwardButton->setToolTip("Next");
    skipForwardButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    connect(skipForwardButton, &QAbstractButton::clicked, this, &MusicPlayer::playNextSong);

    skipBackButton = new QToolButton(this);
    skipBackButton->setEnabled(false);
    skipBackButton->setToolTip("Back");
    skipBackButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    connect(skipForwardButton, &QAbstractButton::clicked, this, &MusicPlayer::playLastSong);


    QAbstractButton *openButton = new QToolButton(this);
    openButton->setText(tr("..."));
    openButton->setToolTip(tr("Open a file..."));
    openButton->setFixedSize(playButton->sizeHint());
    connect(openButton, &QAbstractButton::clicked, this, &MusicPlayer::openFile);

    volumeButton = new VolumeButton(this);
    volumeButton->setToolTip(tr("Adjust volume"));
    volumeButton->setVolume(mediaPlayer.volume());
    connect(volumeButton, &VolumeButton::volumeChanged, &mediaPlayer, &QMediaPlayer::setVolume);

    positionSlider = new QSlider(Qt::Horizontal, this);
    positionSlider->setEnabled(false);
    positionSlider->setToolTip(tr("Seek"));
    connect(positionSlider, &QAbstractSlider::valueChanged, this, &MusicPlayer::setPosition);

    infoLabel = new QLabel(this);
    positionLabel = new QLabel(tr("00:00"), this);
    positionLabel->setMinimumWidth(positionLabel->sizeHint().width());

    blessButton = new QPushButton("BLESS",this);
    blessButton->setIcon(QIcon(":/image/rose_3.png"));
    blessButton->setStyleSheet("QPushButton {font: 10pt '幼圆'; background:transparent; border-radius: 3px; color: rgb(0,0,0); max-width:70px; }"
                               "QPushButton:hover { font:  11pt '幼圆'}");


    connect(blessButton, &QPushButton::clicked, this, &MusicPlayer::respondToBlessClicked);
    QHBoxLayout *blessLayout = new QHBoxLayout(this);
    blessLayout->setMargin(0);
    blessLayout->addWidget(infoLabel);
    blessLayout->addWidget(blessButton);
    QWidget *w = new QWidget(this);
    w->setLayout(blessLayout);


    songListWidget = new QListWidget(this);
    songListWidget->setToolTip(tr("Songslist"));
    connect(songListWidget, &QListWidget::itemDoubleClicked, this, &MusicPlayer::playSelectedSong);

    QHBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setMargin(0);
    controlLayout->addWidget(openButton);
    controlLayout->addWidget(skipBackButton);
    controlLayout->addWidget(playButton);
    controlLayout->addWidget(skipForwardButton);
    controlLayout->addWidget(positionSlider);
    controlLayout->addWidget(positionLabel);
    controlLayout->addWidget(volumeButton);

    QBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(w);
    mainLayout->addWidget(songListWidget);
    mainLayout->addLayout(controlLayout);
}

void MusicPlayer::createShortcuts()
{
    QShortcut *quitShortcut = new QShortcut(QKeySequence::Quit, this);
    connect(quitShortcut, &QShortcut::activated, QCoreApplication::quit);

    QShortcut *openShortcut = new QShortcut(QKeySequence::Open, this);
    connect(openShortcut, &QShortcut::activated, this, &MusicPlayer::openFile);

    QShortcut *toggleShortcut = new QShortcut(Qt::Key_Space, this);
    connect(toggleShortcut, &QShortcut::activated, this, &MusicPlayer::togglePlayback);

    QShortcut *forwardShortcut = new QShortcut(Qt::Key_Right, this);
    connect(forwardShortcut, &QShortcut::activated, this, &MusicPlayer::seekForward);

    QShortcut *backwardShortcut = new QShortcut(Qt::Key_Left, this);
    connect(backwardShortcut, &QShortcut::activated, this, &MusicPlayer::seekBackward);

    QShortcut *increaseShortcut = new QShortcut(Qt::Key_Up, this);
    connect(increaseShortcut, &QShortcut::activated, volumeButton, &VolumeButton::increaseVolume);

    QShortcut *decreaseShortcut = new QShortcut(Qt::Key_Down, this);
    connect(decreaseShortcut, &QShortcut::activated, volumeButton, &VolumeButton::descreaseVolume);
}

//! [4]
void MusicPlayer::createJumpList()
{
    QWinJumpList jumplist;
    jumplist.recent()->setVisible(true);
}
//! [4]

//! [5]
void MusicPlayer::createTaskbar()
{
    taskbarButton = new QWinTaskbarButton(this);
    taskbarButton->setWindow(windowHandle());

    taskbarProgress = taskbarButton->progress();
    connect(positionSlider, &QAbstractSlider::valueChanged, taskbarProgress, &QWinTaskbarProgress::setValue);
    connect(positionSlider, &QAbstractSlider::rangeChanged, taskbarProgress, &QWinTaskbarProgress::setRange);

    connect(&mediaPlayer, &QMediaPlayer::stateChanged, this, &MusicPlayer::updateTaskbar);
}
//! [5]

//! [6]
void MusicPlayer::createThumbnailToolBar()
{
    thumbnailToolBar = new QWinThumbnailToolBar(this);
    thumbnailToolBar->setWindow(windowHandle());

    playToolButton = new QWinThumbnailToolButton(thumbnailToolBar);
    playToolButton->setEnabled(false);
    playToolButton->setToolTip(tr("Play"));
    playToolButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    connect(playToolButton, &QWinThumbnailToolButton::clicked, this, &MusicPlayer::togglePlayback);

    forwardToolButton = new QWinThumbnailToolButton(thumbnailToolBar);
    forwardToolButton->setEnabled(false);
    forwardToolButton->setToolTip(tr("Fast forward"));
    forwardToolButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
    connect(forwardToolButton, &QWinThumbnailToolButton::clicked, this, &MusicPlayer::seekForward);

    backwardToolButton = new QWinThumbnailToolButton(thumbnailToolBar);
    backwardToolButton->setEnabled(false);
    backwardToolButton->setToolTip(tr("Rewind"));
    backwardToolButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
    connect(backwardToolButton, &QWinThumbnailToolButton::clicked, this, &MusicPlayer::seekBackward);

    thumbnailToolBar->addButton(backwardToolButton);
    thumbnailToolBar->addButton(playToolButton);
    thumbnailToolBar->addButton(forwardToolButton);

    connect(&mediaPlayer, &QMediaPlayer::positionChanged, this, &MusicPlayer::updateThumbnailToolBar);
    connect(&mediaPlayer, &QMediaPlayer::durationChanged, this, &MusicPlayer::updateThumbnailToolBar);
    connect(&mediaPlayer, &QMediaPlayer::stateChanged, this, &MusicPlayer::updateThumbnailToolBar);
}
//! [6]

void MusicPlayer::respondToBlessClicked()
{
    happymark = !happymark;
    if(happymark) {
        mediaPlayer.stop();
        rose->show();
        happyplayer->play();
    } else {
        rose->close();
        happyplayer->stop();
        mediaPlayer.play();
    }
}

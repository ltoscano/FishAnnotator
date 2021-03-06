#include "fish_annotator/video_annotator/player.h"

namespace fish_annotator { namespace video_annotator {

Player::Player(QObject *parent)
  : QThread(parent)
{
  frameIndex = 0;
  stop = true;
}

bool Player::loadVideo(const std::string &filename) {
  capture.reset(new cv::VideoCapture(filename));
  if (capture->isOpened()) {
    frameRate = capture->get(CV_CAP_PROP_FPS);
    currentSpeed = 100;
    delay = (1000/frameRate);
    return true;
  }
  return false;
}

void Player::Play()
{
  if (!isRunning())
  {
    if (isStopped())
    {
      stop = false;
    }
    start(LowPriority);
  }
}

void Player::run()
{
  while(!stop) {
    if (!capture->read(frame))
    {
      stop = true;
    }
    frameIndex = capture->get(CV_CAP_PROP_POS_FRAMES);
    if (frame.channels() == 3)
    {
      cv::cvtColor(frame, RGBframe, CV_BGR2RGB);
      img = QImage((const unsigned char*)(RGBframe.data),
        RGBframe.cols, RGBframe.rows, QImage::Format_RGB888);
    }
    else
    {
      img = QImage((const unsigned char*)(frame.data),
        frame.cols, frame.rows, QImage::Format_Indexed8);
    }
    emit processedImage(img);
    this->msleep(delay);
  }
}

QImage Player::getOneFrame()
{
  capture->read(frame);
  if (frame.channels() == 3)
  {
    cv::cvtColor(frame, RGBframe, CV_BGR2RGB);
    img = QImage((const unsigned char*)(RGBframe.data),
      RGBframe.cols, RGBframe.rows, QImage::Format_RGB888);
  }
  else
  {
    img = QImage((const unsigned char*)(frame.data),
      frame.cols, frame.rows, QImage::Format_Indexed8);
  }
  return img;
}

Player::~Player()
{
  if (1)
  {
    QMutexLocker locker(&mutex);
    stop = true;
    //capture->release();
    condition.wakeOne();
    wait();
  }
}
void Player::Stop()
{
  stop = true;
}

void Player::msleep(int ms)
{
  std::this_thread::sleep_for(std::chrono::microseconds(ms*1000));
}

bool Player::isStopped() const{
  return this->stop;
}

void Player::speedUp()
{
  delay = delay/2;
  if (delay<1)
    delay = 1;
  currentSpeed = currentSpeed * 2;
}

void Player::slowDown()
{
  delay = delay*2;
  currentSpeed = currentSpeed / 2;
}

QImage Player::nextFrame() {
  setCurrentFrame(++frameIndex);
  return getOneFrame();
}

QImage Player::prevFrame() {
  setCurrentFrame(--frameIndex);
  return getOneFrame();
}

std::int64_t Player::getCurrentFrame()
{
  return frameIndex;
}

double Player::getNumberOfFrames()
{
  return capture->get(CV_CAP_PROP_FRAME_COUNT);
}

double Player::getFrameRate()
{
  return frameRate;
}

void Player::setCurrentFrame(std::int64_t frameNumber)
{
  capture->set(CV_CAP_PROP_POS_MSEC, (double(frameNumber)/double(frameRate)*1000));
  //capture->set(CV_CAP_PROP_POS_FRAMES, frameNumber);
  if (frameNumber > 0)
		frameIndex = capture->get(CV_CAP_PROP_POS_FRAMES);
	else
		frameIndex = 0;
  //frameIndex = frameNumber;
}

QImage Player::setFrame(std::int64_t frame)
{
  setCurrentFrame(frame);
  return getOneFrame();
}


double Player::getCurrentSpeed()
{
  return currentSpeed;
}

void Player::write_image(QString filename) {
	cv::Mat matTemp = frame;
	cv::imwrite(filename.toStdString(), matTemp);
}

#include "../../include/fish_annotator/video_annotator/moc_player.cpp"

}} // namespace fish_annotator::gui

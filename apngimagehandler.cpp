#include "apngimagehandler.h"
#include <QFileDevice>
#include <loadapng.h>

void apngCleanupHandler(void *info);

class ApngImageHandlerPrivate
{
public:
    ApngImageHandlerPrivate()
        : _animated(false)
        , _loaded(false) {}

    void check(QIODevice* device) {
        _loaded = check_apng(device, _defaultSize, _animated) != 0;
    }

    QSize _defaultSize;
    bool _animated;
    bool _loaded;
};

ApngImageHandler::ApngImageHandler() :
	QImageIOHandler(),
	_index(0),
    _data(),
    d(new ApngImageHandlerPrivate())
{}

QByteArray ApngImageHandler::name() const
{
	return "apng";
}

bool ApngImageHandler::canRead() const
{
    return !_data.isEmpty() && !_data[0].first.isNull();
}

bool ApngImageHandler::read(QImage *image)
{
	if(_data.isEmpty())
		return false;
	*image = _data[_index].first;
	return jumpToNextImage() && !image->isNull();
}

QVariant ApngImageHandler::option(QImageIOHandler::ImageOption option) const
{
    if(!d->_loaded)
        d->check(device());
	switch(option) {
	case QImageIOHandler::Size:
	{
        return d->_defaultSize;
	}
	case QImageIOHandler::IncrementalReading:
	case QImageIOHandler::Animation:
        return d->_animated;
	default:
		return QVariant();
	}
}

bool ApngImageHandler::supportsOption(QImageIOHandler::ImageOption option) const
{
	switch(option) {
	case QImageIOHandler::Size:
	case QImageIOHandler::IncrementalReading:
	case QImageIOHandler::Animation:
		return true;
	default:
		return false;
	}
}

bool ApngImageHandler::jumpToNextImage()
{
	if(_index < _data.size() - 1) {
		++_index;
		return true;
	} else
		return false;
}

bool ApngImageHandler::jumpToImage(int imageNumber)
{
	if(imageNumber < _data.size() - 1) {
		_index = imageNumber;
		return true;
	} else
		return false;
}

int ApngImageHandler::loopCount() const
{
	return -1;
}

int ApngImageHandler::imageCount() const
{
	return _data.size();
}

int ApngImageHandler::nextImageDelay() const
{
	if(_data.isEmpty())
		return 0;
	else
		return _data[_index].second;
}

int ApngImageHandler::currentImageNumber() const
{
	return _index;
}

void ApngImageHandler::loadImage()
{
	if(device() != nullptr) {
		std::vector<APNGFrame> frames;
		auto res = load_apng(device(), frames);

		if(res >= 0) {
			_data = QVector<ImageInfo>((int)frames.size());
			for(auto i = 0, max = _data.size(); i < max; ++i) {
				APNGFrame &frame = frames[i];
				QImage image(frame.p, frame.w, frame.h, QImage::Format_RGBA8888, apngCleanupHandler, new APNGFrame(frame));
				_data[i] = {image, qRound(((double)frame.delay_num / (double)frame.delay_den) * 1000.0)};
			}
		}
	}
}

void apngCleanupHandler(void *info)
{
	auto frame = static_cast<APNGFrame*>(info);
	delete[] frame->rows;
	delete[] frame->p;
	delete frame;
}

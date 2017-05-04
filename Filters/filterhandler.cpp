#include "filterhandler.h"

void FilterHandler::handleProgress(int amount) {
    emit valueChanged(amount);
}

void FilterHandler::run()
{
    {
        emit imageReady(apply());
    }
}

void FilterHandler::loadImage(QImage image, int32_t f_id, int32_t first, int32_t second, int32_t third)
{
    _f_id = f_id;
    _first_param = first;
    _second_param = second;
    _third_param = third;
    _image = image;
}

QImage FilterHandler::apply()
{
    QImage buf;
    switch (_f_id) {
    case 0:
       // QMessageBox::information(0, "Message", "Choose filter first!");
        break;
    case 1:
        buf = filters->rotateImage(_image, _first_param);
        break;
    case 2:
        buf = filters->Watercolor(_image, _first_param);
        break;
    case 3:
        buf = filters->orderedDithering(_image, _first_param);
        break;
    case 4:
        buf = filters->FloydSteinbergFilter(_image, _first_param, _second_param, _third_param, 4);
        break;
    case 5:
        buf = filters->grayScale(_image);
        break;
    case 6:
        buf = filters->Embossing(_image);
        break;
    case 7:
        buf = filters->Scale(_image, _first_param);
        break;
    case 8:
        buf = filters->Negative(_image);
        break;
    case 9:
        buf = filters->Edge(_image, _first_param);
        break;
    case 10:
        buf = filters->Roberts(_image, _first_param);
        break;
    case 11:
        buf = filters->Sobel(_image, _first_param);
        break;
    case 13:
        buf = filters->SimpleBlur(_image);
        break;
    case 14:
        buf = filters->GaussianBlur(_image);
        break;
    case 15:
        buf = filters->BoxBlur(_image);
        break;
    case 16:
        buf = filters->GammaCorrection(_image, _first_param);
        break;
    case 17:
        buf = filters->Sharpness(_image);
        break;
    }
    return buf;
}

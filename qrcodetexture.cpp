/* qrcode.cpp */

#include "qrcodetexture.h"

#include <string>
#include <vector>
#include <iostream>


void QRCodeTexture::_bind_methods() {

	ClassDB::bind_method(D_METHOD("get_content"), &QRCodeTexture::get_content);
	ClassDB::bind_method(D_METHOD("set_content", "content"), &QRCodeTexture::set_content);

	ClassDB::bind_method(D_METHOD("get_border"), &QRCodeTexture::get_border);
	ClassDB::bind_method(D_METHOD("set_border", "border"), &QRCodeTexture::set_border);

	ClassDB::bind_method(D_METHOD("set_ecl", "error_correction_level"), &QRCodeTexture::set_ecl);
	ClassDB::bind_method(D_METHOD("get_ecl"), &QRCodeTexture::get_ecl);

	ClassDB::bind_method(D_METHOD("set_color", "color"), &QRCodeTexture::set_color);
	ClassDB::bind_method(D_METHOD("get_color"), &QRCodeTexture::get_color);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "content", PROPERTY_HINT_MULTILINE_TEXT, "", PROPERTY_USAGE_DEFAULT_INTL), "set_content", "get_content");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "color"), "set_color", "get_color");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "border", PROPERTY_HINT_RANGE, "0,20,1", PROPERTY_USAGE_EDITOR), "set_border", "get_border");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "error_correction_level", PROPERTY_HINT_RANGE, "1,4,1", PROPERTY_USAGE_EDITOR), "set_ecl", "get_ecl");
}

QRCodeTexture::QRCodeTexture() {

	create_from_image(GetImage(content, border), Texture::FLAG_MIPMAPS);
}

Ref<Image> QRCodeTexture::GetImage(String p_string, const int p_border, Color p_color) {

	QrCode::Ecc errCorLvl;
	switch (error_correction_level) {
		case 2:
			errCorLvl = QrCode::Ecc::MEDIUM;
			break;
		case 3:
			errCorLvl = QrCode::Ecc::QUARTILE;
			break;
		case 4:
			errCorLvl = QrCode::Ecc::HIGH;
			break;
		default:
			errCorLvl = QrCode::Ecc::LOW;
			break;
	}

	const char *char_content = p_string.utf8().get_data();
	const QrCode qr = QrCode::encodeText(char_content, errCorLvl);

	PoolVector<uint8_t> data;
	int size = qr.getSize() + 2 * p_border;
	data.resize(size * size * 3);

	PoolVector<uint8_t>::Write wd8 = data.write();
	int index = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			wd8[index++] = qr.getModule(i - p_border, j - p_border) ? (int) (p_color.r * 255) : 255;
			wd8[index++] = qr.getModule(i - p_border, j - p_border) ? (int) (p_color.g * 255) : 255;
			wd8[index++] = qr.getModule(i - p_border, j - p_border) ? (int) (p_color.b * 255) : 255;
		}
	}

	Ref<Image> image = memnew(Image(size, size, false, Image::FORMAT_RGB8, data));
	return image;
}

void QRCodeTexture::set_content(const String& p_string) {
	if (content == p_string) {
		return;
	}
	if (p_string.size() > 1024) {
		WARN_PRINT("QRCodeTexture text is limited to 1024 characters and has been cut.");
	}
	content = p_string.left(1024);
	update_qrcode();
}

String QRCodeTexture::get_content() const {
	return content;
}

Color QRCodeTexture::get_color() const {
	return color;
}

void QRCodeTexture::set_color(const Color &p_color) {
	if (color == p_color) {
		return;
	}
	color = p_color;
	print_line("color set");
	update_qrcode();
}


void QRCodeTexture::set_border(const int &p_border) {
	if (border == p_border) {
		return;
	}
	border = p_border;
	update_qrcode();
}

int QRCodeTexture::get_border() const {
	return border;
}

void QRCodeTexture::set_ecl(const int& p_ecl) {
	if (error_correction_level == p_ecl) {
		return;
	}
	error_correction_level = p_ecl;
	update_qrcode();
}

int QRCodeTexture::get_ecl() const {
	return error_correction_level;
}

void QRCodeTexture::update_qrcode() {
	create_from_image(GetImage(content, border, color), Texture::FLAG_MIPMAPS);
}

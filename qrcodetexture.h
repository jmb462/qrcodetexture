/* qrcodetexture.h */

#ifndef QRCODETEXTURE_H
#define QRCODETEXTURE_H

#include "scene/resources/texture.h"
#include "qrcodegen.hpp"
using qrcodegen::QrCode;

class QRCodeTexture : public ImageTexture {
	GDCLASS(QRCodeTexture, ImageTexture);

protected:
	static void _bind_methods();
	String content = "";
	int border = 1;
	Color color = Color(0, 0, 0);
	void update_qrcode();

public:

	// Error correction level : from 1 = lowest to 4 = highest
	int error_correction_level = 1;
	Ref<Image> GetImage(String p_string, const int p_border=1, Color p_color = Color(0,0,0));
	void set_content(const String &p_string);
	String get_content() const;
	void set_border(const int &p_border);
	void set_color(const Color &p_color);
	Color get_color() const;
	int get_border() const;
	void set_ecl(const int &p_ecl);
	int get_ecl() const;
	QRCodeTexture();
};

#endif // QRCODETEXTURE_H

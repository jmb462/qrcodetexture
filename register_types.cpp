/* register_types.cpp */

#include "register_types.h"

#include "core/class_db.h"
#include "qrcodetexture.h"

void register_qrcodetexture_types() {
	ClassDB::register_class<QRCodeTexture>();
}

void unregister_qrcodetexture_types() {
	// Nothing to do here in this example.
}

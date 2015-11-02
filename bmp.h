#include <cstdint>
#include <string>

//Quelle: Wikipedia (http://de.wikipedia.org/wiki/Windows_Bitmap)
struct bmp_header //12Byte
{
	//uint16_t bfType; //= "BM" bzw. 0x42 0x4D bzw. 19778 => in extra variable gelesen
	uint32_t bfSize; //Größe der BMP datei, unzuverlässig
	uint32_t bfReserved; //von Software abhängig, meist 0
	uint32_t bfOffBits; //meist 54, GIMP macht 7A, also 122 und beginn der eigentlichen Bilddaten
};

struct bmp_info //40Byte, bei GIMP 108, aber die ersten 40 sind gleich und der rest ist egal
{
	uint32_t biSize; //Größe des Infoheaders, sollte 40 sein
	int32_t  biWidth; // Breite der Bitmap in Pixel
	int32_t  biHeight; //Höhe in Pixel, falls Positiv: lesen von unterster Zeile an nach oben gehend, falls negativ: von oberster Zeile an bis zur untersten
	uint16_t biPlanes; //1, nicht mehr verwendet
	uint16_t biBitCount; //Farbtiefe in Bits, 1,4,8,16,24 oder 32
	uint32_t biCompression; //0 für 24, kann 1,2,3 für andere BitCounts sein
	uint32_t biSizeImage; //Größe der Bilddaten in Bytes, falls unkomprimiert kann es 0 sein
	int32_t  biXPelsPerMeter; //unwichtig
	int32_t  biYPelsPerMeter; //unwichtig
	uint32_t biClrUsed; //Anzahl Farben in Farbtabelle
	uint32_t biClrImportant; //Anzahl aller verwendeten Farben, falls keine Farbtabelle dann 0
};
char* loadbmp(std::string path, int &width, int &height);

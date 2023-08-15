#include "image.h"
#include "window.h"

Image::Image(Control* parent, std::string const& imgPath, float x, float y, float width, float height)
: Rect(parent, x, y, width, height)
{
	resource.imageFactory->CreateDecoderFromFilename(toWString(imgPath), NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
	decoder->GetFrame(0, &frame);
	resource.imageFactory->CreateFormatConverter(&formatConverter);
	formatConverter->Initialize(frame, GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeMedianCut
	);
	resource.renderTarget->CreateBitmapFromWicBitmap(formatConverter, NULL, &bitmap);
}

void Image::draw()
{
	resource.window->saveRenderState();
	for (Control* clipSource : clipSources)
	{
		Rect* source = static_cast<Rect*>(clipSource);
		resource.renderTarget->SetTransform(source->transform);
		resource.renderTarget->PushAxisAlignedClip(source->rect, D2D1_ANTIALIAS_MODE_ALIASED);
	}
	resource.window->restoreRenderState();

	resource.renderTarget->DrawBitmap(bitmap, rect);

	for (int i = 0; i < clipSources.size(); i++)
	{
		resource.renderTarget->PopAxisAlignedClip();
	}
}
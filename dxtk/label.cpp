#include "label.h"
#include "window.h"

Label::Label(Control* parent, float x, float y, float width, float height)
: Rect(parent, x, y, width, height), scale(1.0f), textLayout(NULL)
{
	resource.dwriteFactory->CreateTextFormat(
    	L"Consolas",
    	NULL,
    	DWRITE_FONT_WEIGHT_NORMAL,
    	DWRITE_FONT_STYLE_NORMAL,
    	DWRITE_FONT_STRETCH_NORMAL,
    	14.0f,
    	L"", //locale
    	&textFormat
    );
	textFormat->SetTextAlignment(TextAlignCenter);
    textFormat->SetParagraphAlignment(ParagraphAlignCenter);
    paragraphAlignment = ParagraphAlignCenter;
    textAlignment = TextAlignCenter;
    text = L"";
}

void Label::setScale(float scale)
{
	this->scale = scale;
	requestRedraw();
}

void Label::setText(std::string const& text)
{
	this->text = toWString(text);
	requestRedraw();
}

void Label::setWrapMode(DWRITE_WORD_WRAPPING wrap)
{
	textFormat->SetWordWrapping(wrap);
	this->wrap = wrap;
	invokeSignal("wrap_mode_changed");
}

void Label::setTextAlignment(DWRITE_TEXT_ALIGNMENT alignment)
{
	textFormat->SetTextAlignment(alignment);
	textAlignment = alignment;
	invokeSignal("text_alignment_changed");
}

void Label::setParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT alignment)
{
	textFormat->SetParagraphAlignment(alignment);
	paragraphAlignment = alignment;
	invokeSignal("paragraph_alignment_changed");
}

void Label::setTextFormat(std::string const& fontFamily, float fontSize, DWRITE_FONT_WEIGHT fontWeight, DWRITE_FONT_STYLE fontStyle)
{
	if (textFormat)
	{
		textFormat->Release();
		textFormat = NULL;
	}

	resource.dwriteFactory->CreateTextFormat(
		toWString(fontFamily),
		NULL,
		fontWeight,
		fontStyle,
		DWRITE_FONT_STRETCH_NORMAL,
		fontSize,
		L"",
		&textFormat
	);
	textFormat->SetTextAlignment(textAlignment);
    textFormat->SetParagraphAlignment(paragraphAlignment);
    invokeSignal("text_format_changed");
}

HitInfo Label::getPointHit(float pointX, float pointY)
{
	if (!textLayout)
	{
		resource.dwriteFactory->CreateTextLayout(
			text,
			(UINT32)wcslen(text),
			textFormat,
			width,
			height,
			&textLayout
		);
	}

	HitInfo hitInfo;

	textLayout->HitTestPoint(
		pointX,
		pointY,
		&hitInfo.isTrailingHit,
		&hitInfo.isInside,
		&hitInfo.metrics
	);

	return hitInfo;
}

HitTestMetrics Label::getFontMetrics(UINT32 textPosition)
{
	if (textLayout)
	{
		textLayout->Release();
		textLayout = NULL;
	}

	resource.dwriteFactory->CreateTextLayout(
		text,
		(UINT32)wcslen(text),
		textFormat,
		width,
		height,
		&textLayout
	);

	// unused, just to satisfy parameters
	float pointX;
	float pointY;

	HitTestMetrics metrics;

	textLayout->HitTestTextPosition(
		textPosition,
		TRUE,
		&pointX,
		&pointY,
		&metrics
	);

	return metrics;
}

void Label::update()
{
	Control::update();
	D2D1::Matrix3x2F transformScale = D2D1::Matrix3x2F::Scale(D2D1::Size(scale, scale), D2D1::Point2F(width/2, height/2));
	D2D1::Matrix3x2F transformTrans = D2D1::Matrix3x2F::Translation(x, y);
	resource.renderTarget->SetTransform(transformScale * transformTrans);
}

void Label::draw()
{
	resource.window->saveRenderState();
	for (Control* clipSource : clipSources)
	{
		Rect* source = static_cast<Rect*>(clipSource);
		resource.renderTarget->SetTransform(source->transform);
		resource.renderTarget->PushAxisAlignedClip(source->rect, D2D1_ANTIALIAS_MODE_ALIASED);
	}
	resource.window->restoreRenderState();

	resource.renderTarget->DrawText(
		text,
		(UINT32)wcslen(text),
		textFormat,
		rect,
		brush
	);

	for (int i = 0; i < clipSources.size(); i++)
	{
		resource.renderTarget->PopAxisAlignedClip();
	}
}
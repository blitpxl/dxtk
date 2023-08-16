#include "label.h"

Label::Label(Control* parent, float x, float y, float width, float height)
: Rect(parent, x, y, width, height), textLayout(NULL)
{
	resource.dwriteFactory->CreateTextFormat(
    	L"Consolas",
    	NULL,
    	DWRITE_FONT_WEIGHT_NORMAL,
    	DWRITE_FONT_STYLE_NORMAL,
    	DWRITE_FONT_STRETCH_NORMAL,
    	14.0f,
    	L"", // locale
    	&textFormat
    );
	textFormat->SetTextAlignment(TextAlignCenter);
    textFormat->SetParagraphAlignment(ParagraphAlignCenter);
    paragraphAlignment = ParagraphAlignCenter;
    textAlignment = TextAlignCenter;
    text = L"";
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

void Label::draw()
{
	pushRectClip();

	resource.renderTarget->DrawText(
		text,
		(UINT32)wcslen(text),
		textFormat,
		rect,
		brush
	);

	popRectClip();
}
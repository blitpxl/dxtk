#include "label.h"

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

void Label::setTextAlignment(DWRITE_TEXT_ALIGNMENT alignment)
{
	textFormat->SetTextAlignment(alignment);
	textAlignment = alignment;
}

void Label::setParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT alignment)
{
	textFormat->SetParagraphAlignment(alignment);
	paragraphAlignment = alignment;
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
}

FontMetrics Label::getFontMetrics(UINT32 textPosition)
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

	float pointX;
	float pointY;
	FontMetrics metrics;

	textLayout->HitTestTextPosition(
		textPosition,
		FALSE,
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
	resource.renderTarget->DrawText(
		text,
		(UINT32)wcslen(text),
		textFormat,
		rect,
		brush
	);
}
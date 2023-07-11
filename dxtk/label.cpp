#include "label.h"

Label::Label(Control* parent, float x, float y, float width, float height)
: Rect(parent, x, y, width, height)
{
	resource.dwriteFactory->CreateTextFormat(
    	L"Consolas",
    	NULL,
    	DWRITE_FONT_WEIGHT_BOLD,
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

void Label::update()
{
	Control::update();
	resource.renderTarget->SetTransform(D2D1::Matrix3x2F::Translation(x, y));
}

void Label::draw()
{
	resource.renderTarget->DrawText(
		text,
		wcslen(text),
		textFormat,
		rect,
		brush
	);
}
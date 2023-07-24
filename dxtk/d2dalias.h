#pragma once

// Type Aliases
#define Color 					D2D1::ColorF
#define ColorType 				D2D1_COLOR_F
#define RectF 					D2D1::RectF
#define RectFType 				D2D1_RECT_F
#define RRect					D2D1_ROUNDED_RECT

// Color Utils
#define RandomF static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX)
#define RandomColor Color(RandomF, RandomF, RandomF)

// Text Wrap
#define Wrap					DWRITE_WORD_WRAPPING_WRAP
#define NoWrap					DWRITE_WORD_WRAPPING_NO_WRAP
#define EmergencyBreak			DWRITE_WORD_WRAPPING_EMERGENCY_BREAK
#define WholeWord				DWRITE_WORD_WRAPPING_WHOLE_WORD
#define WrapCharacter			DWRITE_WORD_WRAPPING_CHARACTER


// Text Alignments
#define TextAlignLeading 		DWRITE_TEXT_ALIGNMENT_LEADING
#define TextAlignTrailing 		DWRITE_TEXT_ALIGNMENT_TRAILING
#define TextAlignCenter 		DWRITE_TEXT_ALIGNMENT_CENTER
#define TextAlignJustified 		DWRITE_TEXT_ALIGNMENT_JUSTIFIED

// Paragraph Alignments
#define ParagraphAlignNear 		DWRITE_PARAGRAPH_ALIGNMENT_NEAR
#define ParagraphAlignFar 		DWRITE_PARAGRAPH_ALIGNMENT_FAR
#define ParagraphAlignCenter	DWRITE_PARAGRAPH_ALIGNMENT_CENTER

// Font Weight
#define FontWeightThin 			DWRITE_FONT_WEIGHT_THIN
#define FontWeightExtraLight 	DWRITE_FONT_WEIGHT_EXTRA_LIGHT
#define FontWeightUltraLight 	DWRITE_FONT_WEIGHT_ULTRA_LIGHT
#define FontWeightLight 		DWRITE_FONT_WEIGHT_LIGHT
#define FontWeightSemiLight 	DWRITE_FONT_WEIGHT_SEMI_LIGHT
#define FontWeightNormal 		DWRITE_FONT_WEIGHT_NORMAL
#define FontWeightRegular 		DWRITE_FONT_WEIGHT_REGULAR
#define FontWeightMedium		DWRITE_FONT_WEIGHT_MEDIUM
#define FontWeightDemiBold 		DWRITE_FONT_WEIGHT_DEMI_BOLD
#define FontWeightSemiBold 		DWRITE_FONT_WEIGHT_SEMI_BOLD
#define FontWeightBold 			DWRITE_FONT_WEIGHT_BOLD
#define FontWeightExtraBold 	DWRITE_FONT_WEIGHT_EXTRA_BOLD
#define FontWeightUltraBold 	DWRITE_FONT_WEIGHT_ULTRA_BOLD
#define FontWeightBlack 		DWRITE_FONT_WEIGHT_BLACK
#define FontWeightHeavy 		DWRITE_FONT_WEIGHT_HEAVY
#define FontWeightExtraBlack 	DWRITE_FONT_WEIGHT_EXTRA_BLACK
#define FontWeightUltraBlack 	DWRITE_FONT_WEIGHT_ULTRA_BLACk

// Font Style
#define FontStyleNormal 		DWRITE_FONT_STYLE_NORMAL
#define FontStyleOblique 		DWRITE_FONT_STYLE_OBLIQUE
#define FontStyleItalic 		DWRITE_FONT_STYLE_ITALIC

// Font Metrics struct
#define FontMetrics				DWRITE_HIT_TEST_METRICS 
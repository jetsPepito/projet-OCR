#ifndef _DOUBLE_IMG_H_
#define _DOUBLE_IMG_H_


typedef struct DoubleInt DoubleInt;
struct DoubleInt
{
	int l;
    int c;
};

typedef struct Rectangle Rectangle;
struct Rectangle
{
	DoubleInt TopLeft;
    DoubleInt TopRight;
    DoubleInt BotLeft;
    DoubleInt BotRight;
};

typedef struct DoubleRect DoubleRect;
struct DoubleRect
{
    Rectangle Rect1;
    Rectangle Rect2;
}

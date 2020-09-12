﻿#include <iostream>

using namespace std;

const char gStageData[] = "\
########\n\
# .. p #\n\
# oo   #\n\
#      #\n\
########";
const int gStageWidth = 8;
const int gStageHeight = 5;

enum Object
{
    OBJ_SPACE,
    OBJ_WALL,
    OBJ_GOAL,
    OBJ_BLOCK,
    OBJ_BLOCK_ON_GOAL,
    OBJ_MAN,
    OBJ_MAN_ON_GOAL,
    OBJ_UNKNOWN,
};

void initialize(Object *state, int w, int h, const char *stageData);
void draw(const Object *state, int w, int h);
void update(Object *state, char input, int w, int h);
bool checkClear(const Object *state, int w, int h);

int main()
{
    Object *state = new Object[gStageWidth * gStageHeight];
    initialize(state, gStageWidth, gStageHeight, gStageData);
    while (true)
    {
        draw(state, gStageWidth, gStageHeight);
        if (checkClear(state, gStageWidth, gStageHeight))
        {
            break;
        }
        cout << "a:left w:up s:down d:right. command?" << endl;
        char input;
        cin >> input;
        update(state, input, gStageWidth, gStageHeight);
    }
    cout << "you win!" << endl;
    state = 0;
    return 0;
}

void initialize(Object *state, int width, int height, const char *stageData)
{
    const char *d = stageData;
    int x = 0;
    int y = 0;
    while (*d != '\0')
    {
        Object t;
        switch (*d)
        {
        case '#':
            t = OBJ_WALL;
            break;
        case ' ':
            t = OBJ_SPACE;
            break;
        case 'o':
            t = OBJ_BLOCK;
            break;
        case 'O':
            t = OBJ_BLOCK_ON_GOAL;
            break;
        case '.':
            t = OBJ_GOAL;
            break;
        case 'p':
            t = OBJ_MAN;
            break;
        case 'P':
            t = OBJ_MAN_ON_GOAL;
            break;
        case '\n':
            x = 0;
            ++y;
            t = OBJ_UNKNOWN;
            break;
        default:
            t = OBJ_UNKNOWN;
        }
        ++d;
        if (t != OBJ_UNKNOWN)
        {
            state[y * width + x] = t;
            ++x;
        }
    }
}

void draw(const Object *state, int width, int height)
{
    const char font[] = {' ', '#', '.', 'o', 'O', 'p', 'P'};
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Object o = state[y * width + x];
            cout << font[o];
        }
        cout << endl;
    }
}

void update(Object *state, char input, int width, int height)
{
    int dx = 0;
    int dy = 0;
    switch (input)
    {
    case 'a':
        dx = -1;
        break;
    case 'd':
        dx = 1;
        break;
    case 'w':
        dy = -1;
        break;
    case 's':
        dy = 1;
        break;
    }
    int i = -1;
    for (i = 0; i < width * height; ++i)
    {
        if (state[i] == OBJ_MAN || state[i] == OBJ_MAN_ON_GOAL)
        {
            break;
        }
    }
    int x = i % width;
    int y = i / width;
    int tx = x + dx;
    int ty = y + dy;
    if (tx < 0 || ty < 0 || tx >= width || ty >= height)
    {
        return;
    }
    int p = y * width + x;
    int tp = ty * width + tx;
    if (state[tp] == OBJ_SPACE || state[tp] == OBJ_GOAL)
    {
        state[tp] = (state[tp] == OBJ_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
        state[p] = (state[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
    }
    else if (state[tp] == OBJ_BLOCK || state[tp] == OBJ_BLOCK_ON_GOAL)
    {
        int tx2 = tx + dx;
        int ty2 = ty + dy;
        if (tx2 < 0 || ty2 < 0 || tx2 >= width || ty2 >= height)
        {
            return;
        }
        int tp2 = ty2 * width + tx2;
        if (state[tp2] == OBJ_SPACE || state[tp2] == OBJ_GOAL)
        {
            state[tp2] = (state[tp2] == OBJ_GOAL) ? OBJ_BLOCK_ON_GOAL : OBJ_BLOCK;
            state[tp] = (state[tp] == OBJ_BLOCK_ON_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
            state[p] = (state[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
        }
    }
}

bool checkClear(const Object *state, int width, int height)
{
    for (int i = 0; i < width * height; ++i)
    {
        if (state[i] == OBJ_BLOCK)
        {
            return false;
        }
    }
    return true;
}

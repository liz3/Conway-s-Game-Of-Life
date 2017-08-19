#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

struct Config
{
    unsigned  windowWidth,
                windowHeight,
                simWidth,
                simHeight,
                quadSize;
    std::string name;
};

#endif // CONFIG_H_INCLUDED

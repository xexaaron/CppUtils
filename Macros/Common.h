#pragma once

#ifndef CONCAT(x, y)
    #define CONCAT(x, y) x##y 
#endif // CONCAT

#ifndef APPEND(x, y)
    #define APPEND(x, y) CONCAT(x, y) 
#endif // APPEND

#ifndef LITERAL(x)
    #define LITERAL(x) #x
#endif // LITERAL

#ifndef BIT(x)
    #define BIT(x) (1 << x) 
#endif // BIT

#ifndef EXPAND_COLOR(color)
    #define EXPAND_COLOR(color) color.r, color.g, color.b, color.a
#endif // EXPAND_COLOR


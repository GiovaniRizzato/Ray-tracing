#include "rtweekend.h"

#include "vec3.h"

#include <iostream>
#include <vector>

int main1 () {
    std::vector<point3> keyFrames;

    //KeyFrame Inicial
    point3 p_inicio(0,0,5);
    int t_inicial = 0;

    //KeyFrame final
    point3 p_final(0,5,0);
    int t_final = 10;

    int steps = t_final -  t_inicial;

    double x_increment = (p_final.e[0] - p_inicio.e[0])/steps;
    double y_increment = (p_final.e[1] - p_inicio.e[1])/steps;
    double z_increment = (p_final.e[2] - p_inicio.e[2])/steps;

    std::cout << "steps:" << steps << '\n';
    std::cout << "x_increment:" << x_increment << '\n';
    std::cout << "y_increment:" << y_increment << '\n';
    std::cout << "z_increment:" << z_increment << '\n';
    point3 p_acm(p_inicio.e[0], p_inicio.e[1], p_inicio.e[2]);

    keyFrames.push_back(p_inicio);
    for(int i=0; i<=steps-2; i++) {
        p_acm.e[0] += x_increment;
        p_acm.e[1] += y_increment;
        p_acm.e[2] += z_increment;
        keyFrames.push_back(p_acm);
    }
    keyFrames.push_back(p_final);

    std::cout << "keyFrames contains:" << '\n';
    for (std::vector<point3>::iterator it=keyFrames.begin(); it!=keyFrames.end(); ++it)
        std::cout << '(' << *it << ')' << '\n';

    return 0;
}

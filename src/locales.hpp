#ifndef LOCALES_HPP
#define LOCALES_HPP

void vertices_locales(float vertices_resta[], float vertices_talud_[], float vertices_locales_talud[], float Xd, float Yd, float& x1_elipse_local, float& y1_elipse_local, float& x2_elipse_local, float& y2_elipse_local)
{
    for (int i = 0; i < 6; i += 3)
    {
        vertices_resta[i] = Xd;
        vertices_resta[i + 1] = Yd;
        vertices_resta[i + 2] = 0;
    }

    for (int i = 0; i < 6; i++)
    {
        vertices_locales_talud[i] = vertices_talud_[i] - vertices_resta[i];
    }

    x1_elipse_local = vertices_locales_talud[0];
    y1_elipse_local = vertices_locales_talud[1];
    x2_elipse_local = vertices_locales_talud[3];
    y2_elipse_local = vertices_locales_talud[4];
}

#endif
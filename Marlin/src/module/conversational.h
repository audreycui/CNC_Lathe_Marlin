#include "../MarlinCore.h"

class Conversational {

    public: 
        static uint8_t taper_angle; 
        static uint8_t radius_radius; 
        static float feed_speed; 
        static float part_diameter; 
        static float x_offset; 
        static float y_offset; 
        static float move_to_x; 
        static float move_to_z; 

        
    Conversational(); 
    void init(); 

    static void on_taper_update();
    static void on_radius_update();

    static void make_taper(); 

    static void make_radius(); 
    static void on_offset_update(float, char); 

    static void on_move_auto_update(); 
    static void auto_go_x(); 
    static void auto_go_z(); 
}; 
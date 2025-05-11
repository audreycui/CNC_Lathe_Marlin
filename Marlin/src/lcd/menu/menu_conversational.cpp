// conversational lathe menu
#include "../../inc/MarlinConfigPre.h"

//#if HAS_MARLINUI_MENU

#include "menu_item.h"
#include "menu_addon.h"

#include "../../module/motion.h"
#include "../../module/planner.h"
#include "../../gcode/gcode.h" // for inch support
#include "../../gcode/queue.h"
#include "../../module/temperature.h"
#include "../../module/conversational.h"

#if ENABLED(DELTA)
  #include "../../module/delta.h"
#endif

// Always show configurable options regardless of FT Motion active
//#define FT_MOTION_NO_MENU_TOGGLE

constexpr bool has_large_area() {
  return TERN0(HAS_X_AXIS, (X_BED_SIZE) >= 1000) || TERN0(HAS_Y_AXIS, (Y_BED_SIZE) >= 1000) || TERN0(HAS_Z_AXIS, (Z_MAX_POS) >= 1000);
}


extern Conversational conversational; 
uint8_t Conversational::taper_angle; 
uint8_t Conversational::radius_radius; 
float Conversational::feed_speed; 
float Conversational::part_diameter; 
float Conversational::x_offset; 
float Conversational::y_offset; 
float Conversational::move_to_x; 
float Conversational::move_to_z; 

extern Planner planner; 
float move_x; 
float move_z; 
float epsilon = 0.01; 
float curr_x;
float curr_z;  


void Conversational::on_offset_update(float offset, char axis){
    char offset_buf[30] = { 0 };
    char str_1[16];
 
    if (axis=='X') sprintf_P(offset_buf, PSTR("G92 X%s"), dtostrf(offset, 1, 3, str_1));
    else if (axis=='Y') sprintf_P(offset_buf, PSTR("G92 Y%s"), dtostrf(offset, 1, 3, str_1));

    gcode.process_subcommands_now(F(offset_buf));

    char offset_debug[30] = {0}; 
    sprintf_P(offset_debug, PSTR("M118 E1 %s"), offset_buf); 
    queue.inject(F(offset_debug)); 
}
float delta_z; 
void Conversational::on_taper_update(){

    get_cartesian_from_steppers();
    xyze_pos_t npos = LOGICAL_AXIS_ARRAY(
      planner.get_axis_position_mm(E_AXIS),
      cartes.x, cartes.y, cartes.z,
      cartes.i, cartes.j, cartes.k,
      cartes.u, cartes.v, cartes.w
    );
    curr_x = npos.asLogical().x/25.4f; 
    curr_z = npos.asLogical().y/25.4f; 

    float delta_x = conversational.part_diameter-curr_x; 
    move_x = conversational.part_diameter + epsilon; 
    delta_z = (epsilon+ delta_x/tan(conversational.taper_angle*1.0*M_PI/180)); 
    move_z = curr_z- delta_z; 
}

void Conversational::make_taper(){
    char buf[30] = { 0 };
    char str_1[16];
    char str_2[16]; 

    sprintf_P(buf, PSTR("M118 E1 %s"), dtostrf(curr_x, 1, 3, str_1));
    gcode.process_subcommands_now(F(buf));

    sprintf_P(buf, PSTR("M118 E1 %s"), dtostrf(curr_z, 1, 3, str_1));
    gcode.process_subcommands_now(F(buf));

    sprintf_P(buf, PSTR("M118 E1 %s"), dtostrf(delta_z, 1, 3, str_1));
    gcode.process_subcommands_now(F(buf));

    char debug_buf[30] = {0}; 
    sprintf_P(debug_buf, PSTR("M118 E1 %s"), buf); 
    gcode.process_subcommands_now(F(debug_buf)); 

    sprintf_P(buf, PSTR("G0 X%s Y%s"), dtostrf(move_x, 1, 3, str_1), dtostrf(move_z, 1, 3, str_2));
    gcode.process_subcommands_now(F(buf));

    sprintf_P(debug_buf, PSTR("M118 E1 %s"), buf); 
    gcode.process_subcommands_now(F(debug_buf)); 
    
}

void Conversational::on_radius_update(){

  get_cartesian_from_steppers();
  xyze_pos_t npos = LOGICAL_AXIS_ARRAY(
    planner.get_axis_position_mm(E_AXIS),
    cartes.x, cartes.y, cartes.z,
    cartes.i, cartes.j, cartes.k,
    cartes.u, cartes.v, cartes.w
  );
  curr_x = npos.asLogical().x/25.4f; 
  curr_z = npos.asLogical().y/25.4f; 

  float delta_x = conversational.part_diameter-curr_x; 
  move_x = curr_x + conversational.radius_radius; 
  move_z = curr_z- conversational.radius_radius; 

}

void Conversational::make_radius(){
  char buf[30] = { 0 };
    char str_1[16];
    char str_2[16]; 
    char str_3[16]; 

    sprintf_P(buf, PSTR("G0 F%s"), dtostrf(conversational.feed_speed, 1, 3, str_1));
    gcode.process_subcommands_now(F(buf));

    char debug_buf[30] = {0}; 
    sprintf_P(debug_buf, PSTR("M118 E1 %s"), buf); 
    gcode.process_subcommands_now(F(debug_buf)); 

    sprintf_P(buf, PSTR("G2 X%s Y%s R%s"), dtostrf(move_x, 1, 3, str_1), dtostrf(move_z, 1, 3, str_2), dtostrf(conversational.radius_radius, 1, 3, str_3));
    gcode.process_subcommands_now(F(buf));

    sprintf_P(debug_buf, PSTR("M118 E1 %s"), buf); 
    gcode.process_subcommands_now(F(debug_buf)); 

}

void Conversational::on_move_auto_update(){

}

void __lcd_move_axis(const AxisEnum axis) {
    if (ui.use_click()) return ui.goto_previous_screen_no_defer();
    if (ui.encoderPosition && !ui.manual_move.processing) {
      // Get motion limit from software endstops, if any
      float min, max;
      soft_endstop.get_manual_axis_limits(axis, min, max);
  
      // Delta limits XY based on the current offset from center
      // This assumes the center is 0,0
      #if ENABLED(DELTA)
        if (axis != Z_AXIS) {
          max = SQRT(FLOAT_SQ(PRINTABLE_RADIUS) - sq(current_position[Y_AXIS - axis])); // (Y_AXIS - axis) == the other axis
          min = -max;
        }
      #endif
  
      // Get the new position
      const float diff = float(int32_t(ui.encoderPosition)) * ui.manual_move.menu_scale;
      (void)ui.manual_move.apply_diff(axis, diff, min, max);
      ui.manual_move.soon(axis);
      ui.refresh(LCDVIEW_REDRAW_NOW);
    }
    ui.encoderPosition = 0;
    if (ui.should_draw()) {
      MenuEditItemBase::itemIndex = axis;
      const float pos = ui.manual_move.axis_value(axis);
     
    const float imp_pos = parser.per_axis_value(axis, pos);
    MenuEditItemBase::draw_edit_screen(GET_TEXT_F(MSG_MOVE_N), ftostr63(imp_pos));
      
    }
  }

  void __goto_manual_move(const_float_t scale) {
    ui.defer_status_screen();
    ui.manual_move.menu_scale = scale;
    ui.goto_screen(ui.manual_move.screen_ptr);
    thermalManager.set_menu_cold_override(true);
  }

  void __menu_move_distance(const AxisEnum axis, const screenFunc_t func, const int8_t eindex=active_extruder) {
    ui.manual_move.screen_ptr = func;
    START_MENU();
    if (LCD_HEIGHT >= 4) {
      if (axis < NUM_AXES)
        STATIC_ITEM_N(axis, MSG_MOVE_N, SS_DEFAULT|SS_INVERT);
      else {
        TERN_(MANUAL_E_MOVES_RELATIVE, ui.manual_move.e_origin = current_position.e);
        STATIC_ITEM_N(eindex, MSG_MOVE_EN, SS_DEFAULT|SS_INVERT);
      }
    }
  
    BACK_ITEM(MSG_MOVE_AXIS_MANUAL);
  
    #define __LINEAR_LIMIT(D) ((D) < max_length(axis) / 2 + 1)
    #if HAS_EXTRUDERS
      #ifndef EXTRUDE_MAXLENGTH
        #define EXTRUDE_MAXLENGTH 50
      #endif
      #define _LINEAR_LIMIT(D) ((axis < E_AXIS) ? __LINEAR_LIMIT(D) : ((D) < (EXTRUDE_MAXLENGTH) / 2 + 1))
    #else
      #define _LINEAR_LIMIT __LINEAR_LIMIT
    #endif
  
    #define __MOVE_SUB(L,T,D) if (rotational[axis] || _LINEAR_LIMIT(D)) SUBMENU_S(F(T), L, []{ __goto_manual_move(D); })

    if (parser.using_inch_units()) {
      #ifdef MANUAL_MOVE_DISTANCE_IN
        #define _MOVE_IN(I) __MOVE_SUB(MSG_MOVE_N_IN, STRINGIFY(I), IN_TO_MM(I));
        MAP(_MOVE_IN, MANUAL_MOVE_DISTANCE_IN)
      #endif
    }
    else {
      #ifdef MANUAL_MOVE_DISTANCE_MM
        #define _MOVE_MM(M) __MOVE_SUB(MSG_MOVE_N_MM, STRINGIFY(M), M);
        MAP(_MOVE_MM, MANUAL_MOVE_DISTANCE_MM)
      #endif
      #if HAS_Z_AXIS
        if (axis == Z_AXIS && (FINE_MANUAL_MOVE) > 0.0f && (FINE_MANUAL_MOVE) < 0.1f)
          SUBMENU_f(F(STRINGIFY(FINE_MANUAL_MOVE)), MSG_MOVE_N_MM, []{ _goto_manual_move(float(FINE_MANUAL_MOVE)); });
      #endif
    }
    END_MENU();
  }

void _menu_taper (){
    START_MENU(); 
    BACK_ITEM(MSG_TAPER); 
    EDIT_ITEM_FAST(uint8, MSG_TAPER_ANGLE, &conversational.taper_angle, 0, 90, conversational.on_taper_update); 
    EDIT_ITEM_FAST(float32, MSG_PART_DIAMETER, &conversational.part_diameter, 0, 10, conversational.on_taper_update); 
    EDIT_ITEM_FAST(float31, MSG_CONVERSATIONAL_FEED, &conversational.feed_speed, 0, 100, conversational.on_taper_update); 

    ACTION_ITEM(MSG_MAKE_TAPER, conversational.make_taper); 
    
    END_MENU(); 
}

void _menu_radius (){
    START_MENU(); 
    BACK_ITEM(MSG_RADIUS); 
    EDIT_ITEM_FAST(uint8, MSG_RADIUS_RADIUS, &conversational.radius_radius, 0, 90, conversational.on_radius_update); 
    EDIT_ITEM_FAST(float32, MSG_PART_DIAMETER, &conversational.part_diameter, 0, 10, conversational.on_radius_update); 
    EDIT_ITEM_FAST(float31, MSG_CONVERSATIONAL_FEED, &conversational.feed_speed, 0, 100, conversational.on_radius_update); 

    ACTION_ITEM(MSG_MAKE_RADIUS, conversational.make_radius); 

    END_MENU(); 
}

void _menu_offsets(){
    START_MENU(); 
    BACK_ITEM(MSG_SET_OFFSETS); 

    EDIT_ITEM_FAST(float52sign, MSG_X_OFFSET, &conversational.x_offset, -10, 10,  []{ conversational.on_offset_update(conversational.x_offset, 'X');}); 
    EDIT_ITEM_FAST(float52sign, MSG_Y_OFFSET, &conversational.y_offset, -20, 20,  []{ conversational.on_offset_update(conversational.y_offset, 'Y');}); 

    END_MENU(); 
}

void _menu_move(){
    START_MENU();
    BACK_ITEM(MSG_MOVE_AXIS_MANUAL); 
    SUBMENU_N(X_AXIS, MSG_MOVE_N, []{ __menu_move_distance(X_AXIS, []{ __lcd_move_axis(X_AXIS); }); });
    SUBMENU_N(Y_AXIS, MSG_MOVE_N, []{ __menu_move_distance(Y_AXIS, []{ __lcd_move_axis(Y_AXIS); }); });

    END_MENU(); 
}

void Conversational::auto_go_x(){
    char buf[30] = { 0 };
    char str_1[16];
    char str_2[16]; 

    sprintf_P(buf, PSTR("G0 X%s"), dtostrf(conversational.move_to_x, 1, 3, str_1));
    gcode.process_subcommands_now(F(buf));

    //FILL IN WITH GCODE!
}

void Conversational::auto_go_z(){
    char buf[30] = { 0 };
    char str_1[16];
    char str_2[16]; 

    sprintf_P(buf, PSTR("G0 Y%s"), dtostrf(conversational.move_to_z, 1, 3, str_1));
    gcode.process_subcommands_now(F(buf));

    //FILL IN WITH GCODE!
}

void _menu_move_auto_x(){
  START_MENU(); 
  BACK_ITEM(MSG_MOVE_AXIS_AUTO); 
  EDIT_ITEM_FAST(float52sign, MSG_MOVE_X, &conversational.move_to_x, -10, 10, conversational.on_move_auto_update); 
  ACTION_ITEM(MSG_MOVE_AXIS_AUTO_GO, conversational.auto_go_x); 
  END_MENU(); 
}

void _menu_move_auto_z(){
  START_MENU(); 
  BACK_ITEM(MSG_MOVE_AXIS_AUTO); 
  EDIT_ITEM_FAST(float52sign, MSG_MOVE_Y, &conversational.move_to_z, -10, 10, conversational.on_move_auto_update); 
  ACTION_ITEM(MSG_MOVE_AXIS_AUTO_GO, conversational.auto_go_z); 
  END_MENU(); 
}

void _menu_move_auto(){

    START_MENU(); 
    BACK_ITEM(MSG_MOVE_AXIS_AUTO); 
    SUBMENU_N(X_AXIS, MSG_MOVE_N, _menu_move_auto_x); 
    SUBMENU_N(Y_AXIS, MSG_MOVE_N, _menu_move_auto_z); 
    END_MENU(); 
}
void menu_conversational() {
    START_MENU();
    BACK_ITEM(MSG_CONVERSATIONAL); 
    SUBMENU(MSG_SET_OFFSETS, _menu_offsets); 
    SUBMENU(MSG_MOVE_AXIS_MANUAL, _menu_move);
    SUBMENU(MSG_MOVE_AXIS_AUTO, _menu_move_auto); //competition
    SUBMENU(MSG_TAPER, _menu_taper );
    SUBMENU(MSG_RADIUS, _menu_radius );
    END_MENU (); 
}

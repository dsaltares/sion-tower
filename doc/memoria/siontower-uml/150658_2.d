format 74

classinstance 128002 class_ref 128002 // Jugador
  name ""   xyz 13 16 2000 life_line_z 2000
classinstance 128130 class_ref 144770 // System
  name ""   xyz 284 14 2000 life_line_z 2000
durationcanvas 128258 classinstance_ref 128130 // :System
  xyzwh 309 83 2010 11 61
end
durationcanvas 128386 classinstance_ref 128002 // :Jugador
  xyzwh 35 85 2010 11 174
end
durationcanvas 128514 classinstance_ref 128130 // :System
  xyzwh 309 196 2010 11 56
end
msg 128642 asynchronous
  from durationcanvas_ref 128386
  to durationcanvas_ref 128514
  yz 197 2015 msg operation_ref 137730 // "SeleccionarReintentar()"
  show_full_operations_definition default drawing_language default show_context_mode default
  label_xy 105 186
msg 128770 asynchronous
  from durationcanvas_ref 128386
  to durationcanvas_ref 128258
  yz 85 2015 msg operation_ref 131074 // "PantallaVictoria()"
  show_full_operations_definition default drawing_language default show_context_mode default
  label_xy 125 70
msg 128898 return
  from durationcanvas_ref 128514
  to durationcanvas_ref 128386
  yz 241 2015 explicitmsg "CambiarPantalla(Juego)"
  show_full_operations_definition default drawing_language default show_context_mode default
  label_xy 105 227
msg 129026 return
  from durationcanvas_ref 128258
  to durationcanvas_ref 128386
  yz 133 2015 explicitmsg "Estadisticas"
  show_full_operations_definition default drawing_language default show_context_mode default
  label_xy 141 118
end

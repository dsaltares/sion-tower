format 74

classinstance 128002 class_ref 128002 // Jugador
  name ""   xyz 10 10 2000 life_line_z 2000
classinstance 128130 class_ref 144770 // System
  name ""   xyz 281 8 2000 life_line_z 2000
durationcanvas 128258 classinstance_ref 128130 // :System
  xyzwh 306 77 2010 11 61
end
durationcanvas 128386 classinstance_ref 128002 // :Jugador
  xyzwh 32 79 2010 11 174
end
durationcanvas 128514 classinstance_ref 128130 // :System
  xyzwh 306 190 2010 11 57
end
msg 128642 asynchronous
  from durationcanvas_ref 128386
  to durationcanvas_ref 128514
  yz 191 2015 msg operation_ref 137858 // "SeleccionarMenu()"
  show_full_operations_definition default drawing_language default show_context_mode default
  label_xy 119 176
msg 128770 asynchronous
  from durationcanvas_ref 128386
  to durationcanvas_ref 128258
  yz 79 2015 msg operation_ref 131074 // "PantallaVictoria()"
  show_full_operations_definition default drawing_language default show_context_mode default
  label_xy 122 64
msg 128898 return
  from durationcanvas_ref 128514
  to durationcanvas_ref 128386
  yz 236 2015 explicitmsg "CambiarPantalla(Menu)"
  show_full_operations_definition default drawing_language default show_context_mode default
  label_xy 105 219
msg 129026 return
  from durationcanvas_ref 128258
  to durationcanvas_ref 128386
  yz 127 2015 explicitmsg "Estadisticas"
  show_full_operations_definition default drawing_language default show_context_mode default
  label_xy 138 112
end

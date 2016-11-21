format 74

classinstance 128002 class_ref 128002 // Jugador
  name ""   xyz 12 14 2000 life_line_z 2000
classinstance 128130 class_ref 144770 // System
  name ""   xyz 283 21 2000 life_line_z 2000
durationcanvas 128258 classinstance_ref 128002 // :Jugador
  xyzwh 34 109 2010 11 174
end
durationcanvas 128386 classinstance_ref 128130 // :System
  xyzwh 308 107 2010 11 61
end
durationcanvas 128770 classinstance_ref 128130 // :System
  xyzwh 308 220 2010 11 56
end
msg 128514 asynchronous
  from durationcanvas_ref 128258
  to durationcanvas_ref 128386
  yz 109 2015 msg operation_ref 131074 // "PantallaVictoria()"
  show_full_operations_definition default drawing_language default show_context_mode default
  label_xy 123 94
msg 128642 return
  from durationcanvas_ref 128386
  to durationcanvas_ref 128258
  yz 157 2015 explicitmsg "Estadisticas"
  show_full_operations_definition default drawing_language default show_context_mode default
  label_xy 139 142
msg 128898 asynchronous
  from durationcanvas_ref 128258
  to durationcanvas_ref 128770
  yz 221 2015 msg operation_ref 128514 // "SeleccionarNivel()"
  show_full_operations_definition default drawing_language default show_context_mode default
  label_xy 120 206
msg 129026 return
  from durationcanvas_ref 128770
  to durationcanvas_ref 128258
  yz 265 2015 explicitmsg "CambiarPantalla(SeleccionNivel)"
  show_full_operations_definition default drawing_language default show_context_mode default
  label_xy 78 250
end

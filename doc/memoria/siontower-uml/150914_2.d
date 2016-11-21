format 74

classinstance 128002 class_ref 144770 // System
  name ""   xyz 279 9 2000 life_line_z 2000
classinstance 128130 class_ref 128002 // Jugador
  name ""   xyz 8 11 2000 life_line_z 2000
durationcanvas 128258 classinstance_ref 128130 // :Jugador
  xyzwh 30 83.5 2010 11 174
end
durationcanvas 128386 classinstance_ref 128002 // :System
  xyzwh 304 81.5 2010 11 61
end
durationcanvas 128514 classinstance_ref 128002 // :System
  xyzwh 304 194.5 2010 11 57
end
msg 128642 return
  from durationcanvas_ref 128514
  to durationcanvas_ref 128258
  yz 240.5 2015 explicitmsg "CambiarPantalla(Menu)"
  show_full_operations_definition default drawing_language default show_context_mode default
  label_xy 104 223.5
msg 128770 return
  from durationcanvas_ref 128386
  to durationcanvas_ref 128258
  yz 131.5 2015 explicitmsg "Créditos"
  show_full_operations_definition default drawing_language default show_context_mode default
  label_xy 137 116.5
msg 128898 asynchronous
  from durationcanvas_ref 128258
  to durationcanvas_ref 128514
  yz 195.5 2015 msg operation_ref 137858 // "SeleccionarMenu()"
  show_full_operations_definition default drawing_language default show_context_mode default
  label_xy 118 180.5
msg 129026 asynchronous
  from durationcanvas_ref 128258
  to durationcanvas_ref 128386
  yz 83.5 2015 msg operation_ref 137986 // "PantallaCreditos()"
  show_full_operations_definition default drawing_language default show_context_mode default
  label_xy 121 68.5
end

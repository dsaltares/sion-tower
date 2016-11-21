format 74

classinstance 128002 class_ref 144770 // System
  name ""   xyz 255 13 2000 life_line_z 2000
classinstance 128130 class_ref 128002 // Jugador
  name ""   xyz 10 13 2000 life_line_z 2000
durationcanvas 128258 classinstance_ref 128002 // :System
  xyzwh 280 89 2010 11 53
end
durationcanvas 128386 classinstance_ref 128130 // :Jugador
  xyzwh 32 89 2010 11 88
end
msg 128514 asynchronous
  from durationcanvas_ref 128386
  to durationcanvas_ref 128258
  yz 90 2015 msg operation_ref 130946 // "MoverCamara()"
  show_full_operations_definition default drawing_language default show_context_mode default
  args "raton"
  label_xy 95 75
msg 128642 return
  from durationcanvas_ref 128258
  to durationcanvas_ref 128386
  yz 131 2015 explicitmsg "Camara en los limites"
  show_full_operations_definition default drawing_language default show_context_mode default
  label_xy 99 115
end

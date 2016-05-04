; this will now resolve to `cdr` and return what you would expect
(display (whatever (whatever '(1 2 3))))

; but this one will throw an unbound label error
(display (like-literally-anything-else '(1 2 3)))

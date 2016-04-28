((cond (eq 1 2)             quote
       (atom (quote (3)))   cdr
       (atom (quote ()))    car) (cdr (quote (5 6 7))))

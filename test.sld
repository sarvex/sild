(define reverse
 (lambda (l acc)
  (cond l (reverse (cdr l) (cons (car l) acc))
  acc)))


(display
    (reverse '(1 2 3) '())
)

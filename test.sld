(define conss
 (lambda (x)
  (lambda (y)
   (cons x y))))

(define cons1
    (conss '1)
)

(display
    (cons1 '(2))
)

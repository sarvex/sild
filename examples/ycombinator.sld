(define Y
 (lambda (f)
  ((lambda (x) (f (lambda (y) ((x x) y))))
   (lambda (x) (f (lambda (y) ((x x) y)))))))


(define id (lambda (x) x))

(display (Y id))
(display (id (Y id)))

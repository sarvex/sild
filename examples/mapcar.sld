(define mapcar (lambda (f l)
 (cond l (cons (f (car l))
               (mapcar f (cdr l)))
       '())))

(define wrap
    (lambda (x) (cons x '())))

(define wrapper
    (lambda (l)
        (mapcar wrap l)))

(display
    (wrapper '(1 2 3))
)

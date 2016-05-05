(define neucar car)
(define thinger
    (cons '12 (cdr '(1 2 3 4))))

(display thinger)
(define thingeragain thinger)

(display (neucar thingeragain))

; this one will break because eval cleaned up the other one.

; (display (neucar thingeragain))


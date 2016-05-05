(define neucar car)
(define thinger
    (cons '12 '()))

(display thinger)
(define thingeragain thinger)

(display (neucar thingeragain))

; now it works because we are operating on a copy

(display (neucar thingeragain))
(display (neucar thingeragain))
(display (neucar thingeragain))
(display (neucar thingeragain))
(display (neucar thingeragain))
(display (neucar thingeragain))


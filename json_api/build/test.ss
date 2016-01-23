(import (chibi test) (scheme load))
(load "mammon.ss")

(define rules
  (list
    (rule (irregex "a" 'i) 10)
    (rule (irregex "p" 'i) 5)))

(test 15 (score "apple"))
(test 10 (score "tomato"))

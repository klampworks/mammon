(import (scheme r5rs) (srfi 9) (srfi 1))
(load "irregex.scm")
(load "score.ss")

(define rules
  (list
    (rule (irregex "a" 'i) 10)
    (rule (irregex "p" 'i) 5)))

(display
  (score "apple"))
(newline)

(display
  (score "tomoato"))
(newline)

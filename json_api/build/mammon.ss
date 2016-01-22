(import (scheme r5rs) (srfi 9))
(load "irregex.scm")

(define-record-type :rule
  (rule x y)
  rule?
  (x rule-pattern)
  (y rule-score))

(define a
    (rule (irregex "a" 'i) 10))

(define (score t)
  -1)

(display
  (if (irregex-search (rule-pattern a) "apple") (rule-score a) 0)
)

(import (chibi test) (scheme load))
(load "mammon.ss")

;http://synthcode.com/scheme/irregex/
(define rules
  (list
    (rule (irregex "A" 'i) 10)
    (rule (irregex "P" 'i) 5)))

(test 15 (score "apple"))
(test 10 (score "tomato"))

(define rules
  (list
    (rule (irregex "linux" 'i) 10)
    (rule (irregex "gentoo" 'i) 50)))

(test 10 (score
           (string-append
             "...you might as well skip the Xmas celebration completely, and "
             "instead sit in front of your linux computer playing with the "
             "all-new-and-improved linux kernel version.")))

(test 0 (score "tomato"))

(define rules
  (list
    (rule (irregex '(: "linux" eow) 'i) 10)))

(test 10 (score "linux"))

(test 10 (score "archlinux"))

(test 0 (score "linuxxxx"))

(test 0 (score "tomato"))

(define rules
  (list
    (rule (irregex '(or (: bow "arch" eow) (: bow "archlinux" eow)) 'i) 10)))

(test 10 (score "arch"))

(test 0 (score "architecture"))

(test 10 (score "archlinux"))

(test 0 (score "tomato"))

(define rules
  (list
    (rule (irregex '(or (: bow "rhel") (: bow "red" (? " ") "hat" eow)) 'i) 10)))

(test 10 (score "red hat"))

(test 10 (score "redhat"))

(test 10 (score "rhel"))

(test 10 (score "rhel6"))

(test 10 (score "Red Hat Enterprise (rhel)"))

(test 0 (score "redhatt"))

(test 0 (score "tomato"))
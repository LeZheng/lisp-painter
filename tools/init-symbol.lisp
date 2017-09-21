(defun get-symbols ()
    (if (null EXT:*ARGS*)
        (apropos "")
        (apply #'apropos EXT:*ARGS*))
    (EXT:EXIT))

(EXT:SAVEINITMEM "get-symbols.mem"
    :NORC t
    :QUIET t
    :EXECUTABLE t
    :INIT-FUNCTION #'get-symbols)
(EXT:EXIT)

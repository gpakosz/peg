;;; leg-mode.el -- minor mode for editing leg parsers
;;;
;;; THIS SOFTWARE IS PROVIDED 'AS IS'.  USE ENTIRELY AT YOUR OWN RISK.
;;;
;;; Last edited: 2016-07-26 14:14:47 by piumarta on nas.local

(defvar-local leg-mode-active? nil)
(defvar-local leg-mode-timer   nil)
(defvar-local leg-mode-check?  nil)

(defun leg-mode-changed ()
  (when leg-mode-timer (cancel-timer leg-mode-timer))
  (setq leg-mode-timer (run-with-idle-timer 0.2 t 'leg-mode-check))
  (add-hook 'post-command-hook 'leg-mode-enable-check nil t))

(defun leg-mode-enable-check ()
  (setq leg-mode-check? t))

(defun leg-mode-check ()
  (when (and leg-mode-check? (not (region-active-p)))
    (setq leg-mode-check? nil)
    (let ((  active (save-excursion (setq   active (or (search-backward "%}" nil t) -1))))
	  (inactive (save-excursion (setq inactive (or (search-backward "%%" nil t) -1))))
	  (active?  (> active inactive)))
      (unless (eq active? leg-mode-active?)
	(funcall (if active? 'fundamental-mode 'c-mode))
	(leg-mode-changed)
	(and font-lock-mode (not active?) (font-lock-fontify-buffer))
	(setq leg-mode-active? active?)))))

(defun leg-mode ()
  "Minor mode for editing leg parsers."
  (interactive)
  (funcall 'c-mode)
  (unless (assq 'leg-mode-timer minor-mode-alist)
    (setq minor-mode-alist (cons '(leg-mode-timer " LEG") minor-mode-alist)))
  (leg-mode-changed))

(provide 'leg-mode)


(put 'upcase-region 'disabled nil)

;; Put autosave files (ie #foo#) and backup files (ie foo~) in ~/.emacs.d/.
(custom-set-variables
  '(auto-save-file-name-transforms '((".*" "~/.emacs.d/autosaves/\\1" t)))
  '(backup-directory-alist '((".*" . "~/.emacs.d/backups/"))))

;; create the autosave dir if necessary, since emacs won't.
(make-directory "~/.emacs.d/autosaves/" t)

(global-linum-mode 1)

;; set my defualt font for coding
(set-default-font
 "-outline-Courier New-normal-normal-normal-mono-20-*-*-*-c-*-iso8859-1")

;; Turn on tabs
(setq indent-tabs-mode t)
(setq-default indent-tabs-mode t)

;; Bind the TAB key 
(global-set-key (kbd "TAB") 'self-insert-command)

;; Set the tab width
(setq-default default-tab-width 8)
(setq-default tab-width 8)
(setq-default c-basic-indent 8)
(setq-default c-basic-offset 8)

;;;;;;;;;;;;;;;;;
;; APPEARANCE
;;;;;;;;;;;;;;;;;
(setq font-lock-maximum-decoration t)
(setq visible-bell t)
(setq require-final-newline t)
(setq resize-minibuffer-frame t)
(setq column-number-mode t)
(setq-default transient-mark-mode t)
(setq next-line-add-newlines nil)
(setq blink-matching-paren nil)
;;(global-font-lock-mode 1 t)
(blink-cursor-mode -1)
(tool-bar-mode -1)
(tooltip-mode -1)

(set-foreground-color "#dbdbdb")
(set-background-color "#000000")
(set-cursor-color "#ff0000")

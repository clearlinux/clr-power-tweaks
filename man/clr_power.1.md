## SYNOPSIS

**`clr_power`** is a utility to adjust power and performance settings in the
operating system. It is configurable and can enforce power and performance
policies.

**clr_power** \[**-d**|**--debug**\] 

## DESCRIPTION

**`clr_power`** adjusts power and performance in the operating system by
setting runtime kernel and device parameters in the proc(5) filesystem.
**`clr_power`** operates with built-in default values. Values can be added or
changed in user-defined **clr-power-tweaks.conf(5)** file.

**`clr_power`** applies settings and immediately exits after it is started. It
can act as a daemon that enforces settings with the included systemd files. 


## OPTIONS

`-d`, `--debug`
  Display debug/verbose output including built-in values.


## FILES

- *clr-power.service*
  systemd service unit that executes **clr_power**.

- *clr-power.timer*
  systemd timer that periodically executes the *clr-power.service* to enforce
  settings.

- *clr-power-rfkill.service*
  systemd service that stops bluetooth devices at boot to prevent power drain.

- */etc/clr-power-tweaks.conf*
  Optional user-defined configuration file to override or att values. See
  clr-power-tweaks.conf(5) for more information.


## BUGS

See GitHub Issues: <https://github.com/clearlinux/clr-power-tweaks/issues>


## SEE ALSO

**clr-power-tweaks.conf(5)**, **proc(5)**

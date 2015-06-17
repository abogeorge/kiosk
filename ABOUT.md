
 ========================================================== KIOSK ==========================================================

 ===== DESCRIPTION: =====
Kiosk it's a small Windows application that allows users to create a new fully functional desktop with limited capabilities 
and using a given key combo, can move back and forth between desktops.
Within this newly created desktop a new instance of explorer is started to ensure the capabilities of the original desktop.
Also, the capabilities of the user are limited, so he can't acces any power or user switching functions.

 ===== VERSION CHANGES: =====
 
 --- Kyosk ---
1.0 - Alfa
1.1 - Beta
1.2 - Creating the new desktop
1.3 - Reading the started process from console
1.4 - Hardcoded process started
1.5 - Removed the console
1.6 - Added command line arguments
1.7 - Switch desktops at key press
1.8 - Increased performance
1.9 - Removed external dependencies
2.0 - Changed arhitectura
2.1 - Added Logger (easylogging++)
2.2 - IPC through Named Pipes
2.3 - Disabling basic capabilities using registries
2.4 - Added Tray Icon to display notifications
2.5 - Added Documentation

 --- DesktopKeyLogger ---
1.0 - KeyboardHook testing
1.1 - Removed window
1.2 - Removed external files dependencies
1.3 - Sending notifications using BroadcastMessage function
1.4 - Sending notifications using Named Pipes
1.5 - Sending notifications using key combo
1.6 - Reading combo configuration from file
1.7 - Added documentation
1.8 - Reading configuration from registry and file as back up

 --- DesktopTrayIcon ---
1.0 - Functional
1.1 - Added documentation

 --- KeyConfig ---
1.0 - Writing in conf file and exception handling for providing matching keys
1.1 - Reading default values from conf file
1.2 - Writing configuration from registry and file as back up
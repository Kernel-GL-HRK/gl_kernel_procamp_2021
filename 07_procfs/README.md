## Lesson 07 - ProcFS interface and orange pi bootup

* Update your existing sysfs kernel module with procfs API:
* Create folder in procfs file system;
* Create entry that returns module author name;
* Create entry that returns amount of "store" callback calls;
* Create entry that returns amount of "show" callback calls.
* Build image for orange pi zero
* Attach console output from your development board


Reference links:
https://elixir.bootlin.com/linux/latest/source/samples/kobject/kobject-example.c#L131
https://elixir.bootlin.com/linux/latest/source/drivers/char/ds1620.c#L396
https://elixir.bootlin.com/linux/latest/source/drivers/staging/rtl8192u/r8192U_core.c#L602
https://elixir.bootlin.com/linux/latest/source/drivers/macintosh/via-pmu.c#L555

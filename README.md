# HeatTreatmentPilot

In 2007, I participated in a pilot study of heat treated wood. I developed a heat treatment process by conduction which monitors the weight changes during the process depending on the temperature of the oven. I designed a software which was in charge of communicating with a programmable temperature regulator (Omron E5CK) and with an analytical balance (Kern S72). The two main functions of the software were:
-setting up the temperature regulator of the oven safely (according to a predetermined temperature profile) and data acquisition
-querying the regulator at periodic intervals to measure the temperature of the oven and at the same time, querying the balance to measure the weight loss of the heated wood.

Technologies involved: C++/wxWidgets (Microsoft Visual Studio)

The measurements were exploited as part of a thesis presented in 2011 by Dr Mounir Chaouch, entitled “Effect of intensity of treatment on elemental composition and durability of heat-treated wood: development of a prediction marker of resistance to fungi basidiomysetes” (University of Lorraine, France).

## License

The sources files are now available freely under the MIT license. I hope this work will be useful.




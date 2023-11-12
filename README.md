# Unreal Engine mod.io API Plugin (Unofficial)
A lightweight mod.io API Wrapper for Unreal Engine comfortable &amp; easy to use.
See https://mod.io/ for more general info on mod.io.

This Plugin allows you to directly interact with the mod.io API through Unreal Engine Blueprints or C++ Code.
It is designed strictly according to information published on the mod.io API Documention:
https://docs.mod.io

All the listed API Requests can be made through the Plugin.
It's programmed in C++ but meant to be used via Blueprint Visual Scripts.

# This is **not an official** Plugin!
You can find the official Unreal Engine SDK Plugin provided by mod.io over here:
https://github.com/modio/modio-ue

While it's wonderful to have that officially supported and developed SDK coming a lot of features it's often versions behind the latest stable Unreal Engine Version, missing a few API Requests that can be helpful, doesn't give developers total control over how they want to interact with mod.io as well as no official Android Platform support.
That's why I decided to create an Open Source Community Plugin as an unsupported alternative.

# Dependencies / Requirements
This Plugin is coded for Unreal Engine 5.3, but I don't see why the code wouldn't compile for any other Unreal Engine 5 Version or custom Engine release.
The code is very lightweight and there aren't a lot of external dependencies.

**HOWEVER:**

This Plugin makes use of two free & open source Unreal Engine Plugins created by Georgy Treshchev.
The Plugins are called "Runtime Files Downloader" & "Runtime Archiver".
You can find the Plugins on the Github Page of the Creator:
https://github.com/gtreshchev

Georgy Treshchev is working for mod.io, so I hope there's no issue with using these Plugins for this Project as well.
I simply included them along in this repository.

# This is Work-in-Progress
I'm planning to have a Version 1.0 ready by the end of 2023.
Every API Call implemented yet is tested while being implemented. Yet no additional testing has been done, so expect challenges, problems, holdups or errors!
Also so far only about 50% of the API Requests are fully implemented!

What can you expect from Version 1.0?
- Unreal Engine 5.2 / 5.3 / Later support
- Windows and Android support
- Most of the API Requests are fully implemented and tested
- Working with the local Cache (stored Access Token, Media, Modfiles)
- Downloading / Uploading of Modfiles & Media

Some of these are already done (Version 0.7), but others are WiP.

# How to use it in Blueprints
Anywhere in your Unreal Engine project you can access the mod.io API Subsystem:
![Screenshot 2023-10-06 165855](https://github.com/ArvurGmbH/Unreal_Engine-Mod.io_API/assets/147174198/29de911c-7edd-4b21-bcf0-2242c07d971a)


On that you can create mod.io API Connections:
![Screenshot 2023-10-06 165321](https://github.com/ArvurGmbH/Unreal_Engine-Mod.io_API/assets/147174198/aa1d5aa5-44de-41fd-aad8-5bc1c6282bee)

You need to input the mod.io Game ID and the API-Key for your Game on mod.io.
Also you can enable automated Access Token caching, making it even more comfortable to use the API.
Also you can enable the automated storing of the Access Token persistingly on the disk of the User.


Now you can get that newly created Connection, again anywhere in your project:
![Screenshot 2023-10-06 165455](https://github.com/ArvurGmbH/Unreal_Engine-Mod.io_API/assets/147174198/bd770fed-0504-4bd4-9dbd-e5966ae81a16)

You can also create multiple API connections simultaniously. To get a specific Connection you can get it via the Game ID:
![Screenshot 2023-10-06 172519](https://github.com/ArvurGmbH/Unreal_Engine-Mod.io_API/assets/147174198/1b2536c8-a704-4cd4-87f4-871239ed689a)


And from that Object you can call all the functionality for the API:
![Screenshot 2023-10-06 165706](https://github.com/ArvurGmbH/Unreal_Engine-Mod.io_API/assets/147174198/30d78700-7ad5-4766-9fb6-f2c698352220)
![Screenshot 2023-10-06 172111](https://github.com/ArvurGmbH/Unreal_Engine-Mod.io_API/assets/147174198/85ee80e7-83fa-4656-aad3-004dcf5e8d02)


To react to receiving a response for a request you can bind events to the Event Dispatchers prepared for each request:
![Screenshot 2023-10-06 170138](https://github.com/ArvurGmbH/Unreal_Engine-Mod.io_API/assets/147174198/dc95e3fd-3d9f-43b8-bc7c-3ba5be4f4544)


Also the Plugin features caching the data you received for previous requests and you can easily get the cached data:
![Screenshot 2023-10-06 170315](https://github.com/ArvurGmbH/Unreal_Engine-Mod.io_API/assets/147174198/b0479ccf-d921-4bc0-8530-1f420ab6e421)
![Cached](https://github.com/ArvurGmbH/Unreal_Engine-Mod.io_API/assets/147174198/0a9448ab-48ab-4158-b4cc-12d15adcef24)


For some of the Requests as well as some utility Functions like Uploading / Downloading there are Async Actions available:
![UploadZip](https://github.com/ArvurGmbH/Unreal_Engine-Mod.io_API/assets/147174198/feec57fa-5639-4b8d-a907-710a71d070bf)
![AsyncActions](https://github.com/ArvurGmbH/Unreal_Engine-Mod.io_API/assets/147174198/1fe8e686-7770-4edf-ba99-2d1ca2c1b972)


📘 مشروع ESP32 للتحكم في 4 ريلاي باستخدام MQTT وFirebase
📘 ESP32 Project for 4-Relay Control using MQTT & Firebase
🧾 وصف المشروع | Project Description
لوحة ذكية مبنية على ESP32 تتحكم في 4 ريلايات عن طريق:

أزرار فيزيائية

MQTT (مع دعم Home Assistant)

Firebase Realtime Database

تعمل بشكل كامل حتى بدون إنترنت (Offline Mode)

Smart ESP32-based board controlling 4 relays via:

Physical buttons

MQTT (Home Assistant support)

Firebase Realtime Database

Works even without Internet (Offline Mode)

🔌 المكونات المطلوبة | Required Components
القطعة	النوع أو الرقم
ESP32 Dev Board	أي نسخة مثل DOIT أو NodeMCU
4-Channel Relay Module	5V يعمل بتيار منخفض
أزرار Push Button	عدد 4
مقاومات 10K	عدد 4 (لـ Pull-up إن احتجت)
أسلاك توصيل	Jumper Wires
مصدر تغذية 5V	باور بنك أو شاحن USB

Component	Description/Type
ESP32 Dev Board	Any model like DOIT or NodeMCU
4-Channel Relay Module	5V low-level trigger
Push Buttons	4 units
10K Resistors	4 units (for pull-up if needed)
Jumper Wires	As needed
5V Power Supply	USB charger or power bank

🧠 مميزات المشروع | Project Features
يعتمد على MQTT وFirebase للتكامل الذكي

يدعم العمل بدون إنترنت (Offline Mode)

يستخدم AceButton للتعامل مع الأزرار

يدعم Home Assistant تلقائيًا (MQTT Discovery)

الكود منظم وسهل التطوير

Smart integration using MQTT and Firebase

Works even without internet

AceButton library used for smart button handling

Auto-discovery in Home Assistant via MQTT

Well-structured and easy-to-extend code

⚡ التوصيل الكهربي | Wiring
ESP32 Pin	الوظيفة	Function
GPIO 23	Relay 1	Relay 1
GPIO 19	Relay 2	Relay 2
GPIO 18	Relay 3	Relay 3
GPIO 5	Relay 4	Relay 4
GPIO 13	Button 1	Button 1
GPIO 12	Button 2	Button 2
GPIO 14	Button 3	Button 3
GPIO 27	Button 4	Button 4

📝 الريلاي يعمل بمنطق عكسي (LOW = تشغيل)
📝 Relay is active LOW (LOW = ON)

🛠️ المكتبات المطلوبة | Required Libraries
Firebase_ESP_Client

PubSubClient

AceButton

WiFi

🛠️ حمل المكتبات من Arduino IDE Library Manager
🛠️ Install via Arduino IDE Library Manager

🔧 إعداد Firebase | Firebase Setup
أنشئ مشروع جديد على Firebase Console

فعّل Realtime Database (Test Mode)

انسخ الرابط DATABASE_URL

فعّل Email/Password في Authentication

أضف مستخدم جديد واحفظ USER_EMAIL وUSER_PASSWORD

احصل على API_KEY من صفحة الإعدادات

Create new Firebase project

Enable Realtime Database (Test Mode)

Copy DATABASE_URL

Enable Email/Password in Authentication

Create new user & save USER_EMAIL, USER_PASSWORD

Get API_KEY from project settings

📡 إعداد MQTT | MQTT Setup
استخدم Mosquitto أو أي MQTT broker آخر

عدّل القيم: mqtt_server, mqtt_port, mqtt_user, mqtt_password

Home Assistant يكتشف الأجهزة تلقائيًا

Use Mosquitto or any MQTT broker

Set values: mqtt_server, mqtt_port, mqtt_user, mqtt_password

Home Assistant will auto-discover switches

🚀 طريقة التشغيل | How to Run
عدّل إعدادات WiFi وFirebase وMQTT في الكود

ارفع الكود على ESP32

افتح Serial Monitor (115200)

تأكد من ظهور:

✅ WiFi connected

✅ Firebase connected

✅ MQTT connected

تحكم في الريلايات من:

الأزرار الفيزيائية

Firebase Realtime DB

Home Assistant

Update WiFi, Firebase, MQTT configs

Upload the code to ESP32

Open Serial Monitor (115200 baud)

Check:

✅ WiFi connected

✅ Firebase connected

✅ MQTT connected

Control relays using:

Physical buttons

Firebase

Home Assistant

🧪 اختبار الوظائف | Functional Testing
الضغط على زر فيزيائي => يغير الريلاي + يحدث MQTT + Firebase

تغيير الحالة من Home Assistant => ينعكس على الريلاي

قطع الإنترنت => الريلايات تستمر في العمل

عند استرجاع الاتصال => مزامنة تلقائية

Press physical button => Relay state changes + MQTT + Firebase updated

Change state from Home Assistant => ESP32 follows

Internet loss => local control still works

Reconnect => state re-synced

🧠 ملاحظات تقنية | Technical Notes
استخدمنا relayLoaded[] لتتبع تحميل الحالة من Firebase

الكود يحتوي على reconnectMQTT()، publishDiscovery()

يعيد الاتصال تلقائيًا بـ MQTT وFirebase

يرسل حالة الريلايات تلقائيًا عند إعادة الاتصال

relayLoaded[] used to track initial state from Firebase

Contains reconnectMQTT() and publishDiscovery()

Reconnects automatically to MQTT/Firebase

Relay states republished after reconnect

🧊 مستقبل المشروع | Future Ideas
دعم الضغط المطول للأزرار (مثلاً مؤقت أو مشهد)

شاشة OLED صغيرة لعرض الحالة

تحديث OTA

دعم Google Assistant عن طريق IFTTT + Firebase

تطوير النسخة في صندوق (DIN أو Wall Mount)

Long-press support for timer/scene

Small OLED screen for feedback

OTA updates

Google Assistant via IFTTT + Firebase

Commercial-ready version in proper casing

👤 المطور | Developer
👨‍💻 الاسم: محمد عيد

💡 الفكرة، التنفيذ، البرمجة، التوثيق

🔧 المشروع الأساسي لبناء منصة ذكية محلية

👨‍💻 Name: Mohamed Eid

💡 Idea, coding, execution, documentation

🔧 Core project for a smart local platform

🔒 الحقوق | License
مشروع مفتوح المصدر للتعلم والاستخدام الشخصي. الاستخدام التجاري على مسؤوليتك.
Open-source project for learning and personal use. Commercial use at your own risk.

🔁 آخر تحديث: يوليو 2025 | Last Update: July 2025


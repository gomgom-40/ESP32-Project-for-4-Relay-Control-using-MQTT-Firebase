# 🌐 ESP32 Web Dashboard (Firebase Based)  
# لوحة تحكم ويب لـ ESP32 باستخدام Firebase

واجهة تحكم مرنة مصممة باستخدام HTML وFirebase، تتيح لك التحكم في 4 ريلايات (Relays) في الوقت الحقيقي، من خلال المتصفح مباشرة، بدون حاجة لـ MQTT أو VPN.

---

## ✨ Features / الميزات

- 🔐 Firebase Authentication لتسجيل الدخول
- ⚡ التحكم في 4 ريلايات مع عرض الحالة الفورية
- 🌐 تعمل من أي متصفح – كمبيوتر أو موبايل
- 🎨 تصميم عصري + يدعم Light/Dark Mode (مستقبلاً)
- 🔘 زر لإيقاف جميع الريلايات مرة واحدة
- 🚫 لا تعتمد على MQTT أو سيرفر داخلي

---

## 📦 How to Use / طريقة الاستخدام

1. افتح الموقع المستضاف (GitHub Pages أو استضافة أخرى)
2. سجل الدخول باستخدام بريد وكلمة مرور مخزنين مسبقًا (Firebase)
3. تحكم في الريلايات عبر الواجهة

---

## 🔧 Firebase Setup / إعداد Firebase

✅ Create a new Firebase project  
✅ Enable:  
- Realtime Database (Test Mode for testing)
- Authentication (Email/Password)

🛠️ ثم انسخ بيانات مشروعك في الكود داخل `firebaseConfig`:

```js
const firebaseConfig = {
  apiKey: "YOUR_API_KEY",
  authDomain: "YOUR_PROJECT.firebaseapp.com",
  databaseURL: "https://YOUR_PROJECT.firebaseio.com",
  ...
};
```

🧑‍💻 لا تنس تعديل البريد وكلمة المرور (`allowedEmail`, `allowedPassword`) في الكود.

---

## 📁 Project Structure / محتويات المشروع

| File          | Description / الوصف                        |
|---------------|---------------------------------------------|
| `index.html`  | صفحة الويب الكاملة للوحة التحكم             |
| `README.md`   | هذا الملف التوثيقي                         |

---

## 🔒 Security Notes / ملاحظات أمان

- تأكد أن إعدادات Firebase Database تمنع الوصول العام (استخدم Auth rules)
- لا تستخدم حساب Firebase الأساسي داخل الكود العلني

---

## 👤 Developer / المطور

- الاسم: **محمد عيد**
- 📍 المشروع جزء من منظومة ESP32 الذكية
- 🌐 مشروع الريلاي الأساسي: [ESP32 Relay Project](https://github.com/gomgom-40/esp32-relay-project)

---

## 📅 Last Update / آخر تحديث

**July 2025 / يوليو 2025**
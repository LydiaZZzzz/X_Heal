### Patient Interface

## Features

- **Four-phase State Machine**: Accurately counts rehab motions with idle, descending, holding, and rising phases.
- **Real-time UI**: Displays current reps, phase, and hold seconds in a large, clear format.
- **Firebase Integration**: Fetches and displays training target data from Firestore (e.g., `/patients/1234`).
- **Bluetooth Device Support**: Connects to Device A (angle sensor) and Device B (distance sensor) for real-time data.
- **Dynamic Target Matching**: Automatically matches target fields (e.g., `kneeFlexion.targetAngle`, `heelRaise.targetHeight`) based on `motionType`.

## Requirements

- Node.js >= 16.x
- npm or yarn
- A Firebase project (with config)
- A browser that supports Web Bluetooth (e.g., Chrome)

## Getting Started

1. **Clone the repository**
   ```bash
   git clone <your-repo-url>
   cd <project-directory>
   ```

2. **Install dependencies**
   ```bash
   npm install
   # or
   yarn install
   ```

3. **Configure Firebase**
   - Create a `.env.local` file in the project root with your Firebase config:
     ```
     VITE_FIREBASE_API_KEY=your_api_key
     VITE_FIREBASE_AUTH_DOMAIN=your_auth_domain
     VITE_FIREBASE_PROJECT_ID=your_project_id
     VITE_FIREBASE_STORAGE_BUCKET=your_storage_bucket
     VITE_FIREBASE_MESSAGING_SENDER_ID=your_sender_id
     VITE_FIREBASE_APP_ID=your_app_id
     ```
   - Alternatively, you can fill in your config directly in `src/firebaseConfig.js`.

4. **Start the development server**
   ```bash
   npm run dev
   # or
   yarn dev
   ```
   Open [http://localhost:5173](http://localhost:5173) in your browser (port may vary).

## Main Pages & Functionality

- **Motion Counter Page**: Shows reps, phase, and hold seconds, updating in real time from sensor data.
- **Firebase Data Display**: Fetches and displays patient training targets (e.g., target angle/height).
- **Bluetooth Device Connection**: Click "Connect Device" to pair with Device A or B and stream live data.

## Troubleshooting

- **Bluetooth not connecting?**
  - Make sure you are using Chrome or another Web Bluetooth-compatible browser.
  - Ensure the device is powered on and close to your computer.
- **Firebase data not showing?**
  - Double-check your Firebase config and database rules.
- **Page shows `--` for values?**
  - Data may not be loaded; check your data path and field names.

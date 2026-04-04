// ================= FIREBASE CONFIG =================
// ⚠️ YAHAN APNA FIREBASE CONFIG DAALNA HAI
const firebaseConfig = {
  apiKey: "YOUR_API_KEY",
  authDomain: "YOUR_PROJECT.firebaseapp.com",
  projectId: "YOUR_PROJECT_ID",
  appId: "YOUR_APP_ID"
};

// init firebase
firebase.initializeApp(firebaseConfig);
const auth = firebase.auth();

// ================= GOOGLE LOGIN =================
function googleLogin() {
  const provider = new firebase.auth.GoogleAuthProvider();

  auth.signInWithPopup(provider)
    .then((result) => {
      const user = result.user;

      document.getElementById("userInfo").innerHTML =
        "Logged in as: " + user.email;

      console.log(user);
    })
    .catch((error) => {
      console.log(error);
      alert("Login failed ❌");
    });
}

// auto login check
auth.onAuthStateChanged((user) => {
  if (user) {
    document.getElementById("userInfo").innerHTML =
      "Logged in as: " + user.email;
  }
});

// ================= 🔐 SECURE UPLOAD =================
async function uploadFile() {
  const file = document.getElementById("fileInput").files[0];

  if (!file) {
    alert("Select file ❗");
    return;
  }

  try {
    let formData = new FormData();
    formData.append("file", file);

    let res = await fetch("http://127.0.0.1:5000/upload", {
      method: "POST",
      headers: {
        "key": "12345"   // 🔐 backend key
      },
      body: formData
    });

    let data = await res.text();
    alert(data);

  } catch (err) {
    console.log(err);
    alert("Upload error ❌");
  }
}

// ================= BUTTON FUNCTIONS =================
function downloadFile() {
  alert("Download feature 🔥");
}

function showNodes() {
  alert("Nodes active 📡");
}

function showStorage() {
  alert("Storage info 💾");
}

// ================= 🎨 WALLPAPER FEATURE =================
function setWallpaper() {
  const file = document.getElementById("bgInput").files[0];

  if (!file) {
    alert("Select wallpaper ❗");
    return;
  }

  const reader = new FileReader();

  reader.onload = function(e) {
    document.body.style.backgroundImage = `url(${e.target.result})`;
    document.body.style.backgroundSize = "cover";
    document.body.style.backgroundPosition = "center";

    // save in local storage
    localStorage.setItem("bg", e.target.result);
  };

  reader.readAsDataURL(file);
}

// load saved wallpaper
window.onload = function() {
  const bg = localStorage.getItem("bg");

  if (bg) {
    document.body.style.backgroundImage = `url(${bg})`;
    document.body.style.backgroundSize = "cover";
    document.body.style.backgroundPosition = "center";
  }
};

// ================= NETWORK ANIMATION =================
const canvas = document.getElementById("net");
const ctx = canvas.getContext("2d");

canvas.width = 700;
canvas.height = 400;

let nodes = [];

for (let i = 0; i < 15; i++) {
  nodes.push({
    x: Math.random() * canvas.width,
    y: Math.random() * canvas.height,
    dx: (Math.random() - 0.5) * 1.5,
    dy: (Math.random() - 0.5) * 1.5
  });
}

function draw() {
  ctx.clearRect(0, 0, canvas.width, canvas.height);

  // lines
  for (let i = 0; i < nodes.length; i++) {
    for (let j = i + 1; j < nodes.length; j++) {
      let dist = Math.hypot(
        nodes[i].x - nodes[j].x,
        nodes[i].y - nodes[j].y
      );

      if (dist < 120) {
        ctx.strokeStyle = "rgba(56,189,248,0.3)";
        ctx.beginPath();
        ctx.moveTo(nodes[i].x, nodes[i].y);
        ctx.lineTo(nodes[j].x, nodes[j].y);
        ctx.stroke();
      }
    }
  }

  // nodes
  nodes.forEach(n => {
    ctx.beginPath();
    ctx.arc(n.x, n.y, 4, 0, Math.PI * 2);
    ctx.fillStyle = "#38bdf8";
    ctx.fill();

    n.x += n.dx;
    n.y += n.dy;

    if (n.x < 0 || n.x > canvas.width) n.dx *= -1;
    if (n.y < 0 || n.y > canvas.height) n.dy *= -1;
  });

  requestAnimationFrame(draw);
}

draw();
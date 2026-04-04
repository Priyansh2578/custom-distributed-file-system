// ================= LOAD FILE LIST =================
async function loadFiles() {
  try {
    let res = await fetch("http://127.0.0.1:5000/files");
    let files = await res.json();

    let list = document.getElementById("fileList");
    list.innerHTML = "";

    files.forEach(file => {
      let li = document.createElement("li");
      li.textContent = file;
      list.appendChild(li);
    });

  } catch (err) {
    console.log("Error loading files:", err);
  }
}

// ================= BUTTON FUNCTIONS =================
function downloadFile() {
  alert("Download feature coming soon 📥");
}

function showNodes() {
  alert("Nodes active 📡");
}

function showStorage() {
  alert("Storage info 💾");
}

// ================= UPLOAD FUNCTION =================
async function uploadFile() {
  const file = document.getElementById("fileInput").files[0];

  if (!file) {
    alert("Please select a file ❗");
    return;
  }

  let formData = new FormData();
  formData.append("file", file);

  try {
    let res = await fetch("http://127.0.0.1:5000/upload", {
      method: "POST",
      body: formData
    });

    if (!res.ok) {
      throw new Error("Upload failed");
    }

    let data = await res.text();

    loadFiles(); // 🔥 refresh file list
    alert(data);

  } catch (err) {
    console.log(err);
    alert("Error connecting to backend ❌");
  }
}

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

  // draw lines
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

  // draw nodes
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

// ================= AUTO LOAD FILES =================
window.onload = loadFiles;
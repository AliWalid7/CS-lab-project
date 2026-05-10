const express = require("express");
const http = require("http");
const { Server } = require("socket.io");

const app = express();
const server = http.createServer(app);
const io = new Server(server);

app.use(express.static("public"));

const groups = {};

io.on("connection", (socket) => {

    console.log("User connected");

    socket.on("joinGroup", ({ username, group }) => {

        socket.join(group);

        if (!groups[group]) {
            groups[group] = [];
        }

        groups[group].push(username);

        io.to(group).emit("message", {
            user: "System",
            text: `${username} joined the group`
        });
    });

    socket.on("sendMessage", ({ group, user, text }) => {

        io.to(group).emit("message", {
            user,
            text
        });
    });
});

server.listen(3000, () => {
    console.log("Server running on port 3000");
});
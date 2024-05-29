const express = require('express');
const path = require('path');
const open = require('open');
const checkPrivileges = require('./build/Release/checkPrivileges.node');

const app = express();
const port = 3000;

app.use(express.urlencoded({ extended: true }));
app.use(express.static(path.join(__dirname, 'public')));

app.get('/', (req, res) => {
	res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

app.post('/check', (req, res) => {
	const username = req.body.username;
	const privilege = checkPrivileges.checkPrivileges(username);

	if (privilege) {
		res.send(
			`<h1>Пользователь ${username} имеет привилегию ${privilege}</h1><button onclick="window.history.back()">Назад</button>`
		);
	} else {
		res.send(
			`<h1>Пользователя ${username} нет</h1><button onclick="window.history.back()">Назад</button>`
		);
	}
});

app.listen(port, () => {
	console.log(`Server is running at http://localhost:${port}`);
	open(`http://localhost:${port}`);
});

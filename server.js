const mysql = require('mysql2');
const express = require('express');
const path = require('path');
const cors = require('cors');
const app = express();

// Middleware
app.use(express.static(path.join(__dirname, 'public')));
app.use(express.json());
app.use(cors());

// MySQL connection
const db = mysql.createConnection({
    host: 'localhost',
    user: 'root', // Default XAMPP MySQL username
    password: '', // Default XAMPP MySQL password (blank for default setup)
    database: 'iot_db'
});

db.connect(err => {
    if (err) throw err;
    console.log('Connected to MySQL');
});

// Serve index.html
app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

// API routes
app.get('/api/history', (req, res) => {
    db.query('SELECT * FROM history ORDER BY timestamp DESC', (err, results) => {
        if (err) throw err;
        res.json(results);
    });
});

app.post('/api/history', (req, res) => {
    const { entry } = req.body;
    db.query('INSERT INTO history (entry) VALUES (?)', [entry], (err, result) => {
        if (err) throw err;
        res.status(201).send('History added');
    });
});

app.delete('/api/history', (req, res) => {
    db.query('DELETE FROM history', (err, result) => {
        if (err) throw err;
        res.send('History cleared');
    });
});

const PORT = 3000;
app.listen(PORT, () => {
    console.log(`Server is running on http://localhost:${PORT}`);
});

from flask import Flask, request, jsonify
import mysql.connector
from mysql.connector import Error
import os
from functools import wraps

app = Flask(__name__)

# Database configuration
DB_CONFIG = {
    'host': os.getenv('DB_HOST', 'localhost'),
    'database': os.getenv('DB_NAME', 'your_game_db'),
    'user': os.getenv('DB_USER', 'your_username'),
    'password': os.getenv('DB_PASSWORD', 'your_password')
}

# Simple API key authentication
API_KEY = os.getenv('API_KEY', 'your-secret-api-key')

def require_api_key(f):
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if request.headers.get('X-API-Key') != API_KEY:
            return jsonify({'error': 'Invalid API key'}), 401
        return f(*args, **kwargs)
    return decorated_function

def get_db_connection():
    """Create and return a database connection"""
    try:
        connection = mysql.connector.connect(**DB_CONFIG)
        return connection
    except Error as e:
        print(f"Error connecting to MySQL: {e}")
        return None

# For testing purpose
@app.route('/health', methods=['GET'])
def health_check():
    """Health check endpoint"""
    return jsonify({'status': 'healthy', 'message': 'Game API is running'})

@app.route('/api/player/<int:user_id>', methods=['GET'])
@require_api_key
def get_player(user_id):
    """Load player data by user_id"""
    connection = get_db_connection()
    if not connection:
        return jsonify({'error': 'Database connection failed'}), 500
    
    try:
        cursor = connection.cursor(dictionary=True)
        query = "SELECT * FROM Users WHERE user_id = %s"
        cursor.execute(query, (user_id,))
        player = cursor.fetchone()
        
        if player:
            return jsonify({
                'success': True,
                'player': player
            })
        else:
            return jsonify({
                'success': False,
                'error': f'Player with user_id {user_id} not found'
            }), 404
            
    except Error as e:
        return jsonify({'error': f'Database error: {str(e)}'}), 500
    finally:
        if connection.is_connected():
            cursor.close()
            connection.close()

@app.route('/api/player/<int:user_id>', methods=['PUT'])
@require_api_key
def update_player(user_id):
    """Update player data"""
    connection = get_db_connection()
    if not connection:
        return jsonify({'error': 'Database connection failed'}), 500
    
    try:
        data = request.get_json()
        if not data:
            return jsonify({'error': 'No JSON data provided'}), 400
        
        cursor = connection.cursor()
        query = """
            UPDATE Users 
            SET name = %s, lvl = %s, xp = %s, gold = %s, max_hp = %s, 
                strength_stat = %s, agility_stat = %s, armor_stat = %s 
            WHERE user_id = %s
        """
        
        cursor.execute(query, (
            data.get('name'),
            data.get('lvl'),
            data.get('xp'),
            data.get('gold'),
            data.get('max_hp'),
            data.get('strength_stat'),
            data.get('agility_stat'),
            data.get('armor_stat'),
            user_id
        ))
        
        connection.commit()
        
        if cursor.rowcount > 0:
            return jsonify({
                'success': True,
                'message': f'Player {user_id} updated successfully'
            })
        else:
            return jsonify({
                'success': False,
                'error': f'Player with user_id {user_id} not found'
            }), 404
            
    except Error as e:
        return jsonify({'error': f'Database error: {str(e)}'}), 500
    finally:
        if connection.is_connected():
            cursor.close()
            connection.close()

# Allows duplicate names
# Need to think about where to have limitations
@app.route('/api/player', methods=['POST'])
@require_api_key
def create_player():
    """Create a new player"""
    connection = get_db_connection()
    if not connection:
        return jsonify({'error': 'Database connection failed'}), 500
    
    try:
        data = request.get_json()
        if not data:
            return jsonify({'error': 'No JSON data provided'}), 400
        
        name = data['name']
        # Hardcoded standard values, can be changed later
        lvl = 1
        xp = 0
        gold = 0
        max_hp = 100
        strength_stat = 10
        agility_stat = 10
        armor_stat = 10

        cursor = connection.cursor()
        query = """
            INSERT INTO Users (name, lvl, xp, gold, max_hp, strength_stat, agility_stat, armor_stat)
            VALUES (%s, %s, %s, %s, %s, %s, %s, %s)
        """

        cursor.execute(query, (name, lvl, xp, gold, max_hp, strength_stat, agility_stat, armor_stat))
        connection.commit()
        new_user_id = cursor.lastrowid
        
        return jsonify({
            'success': True,
            'message': 'Player created successfully',
            'user_id': new_user_id
        }), 201
        
    except Error as e:
        return jsonify({'error': f'Database error: {str(e)}'}), 500
    finally:
        if connection.is_connected():
            cursor.close()
            connection.close()

@app.route('/api/player/exists/<name>', methods=['GET'])
@require_api_key
def check_player_exists(name):
    """Check if a player with given name exists"""
    connection = get_db_connection()
    if not connection:
        return jsonify({'error': 'Database connection failed'}), 500
    
    try:
        cursor = connection.cursor()
        query = "SELECT COUNT(*) FROM Users WHERE name = %s"
        cursor.execute(query, (name,))
        count = cursor.fetchone()[0]
        
        return jsonify({
            'exists': count > 0,
            'count': count
        })
        
    except Error as e:
        return jsonify({'error': f'Database error: {str(e)}'}), 500
    finally:
        if connection.is_connected():
            cursor.close()
            connection.close()

# Not used right now, will see if needed later
@app.route('/api/players', methods=['GET'])
@require_api_key
def get_all_players():
    """Get all players (with optional pagination)"""
    connection = get_db_connection()
    if not connection:
        return jsonify({'error': 'Database connection failed'}), 500
    
    try:
        page = request.args.get('page', 1, type=int)
        per_page = request.args.get('per_page', 10, type=int)
        offset = (page - 1) * per_page
        
        cursor = connection.cursor(dictionary=True)
        
        # Get total count
        cursor.execute("SELECT COUNT(*) as total FROM Users")
        total = cursor.fetchone()['total']
        
        # Get paginated results
        query = "SELECT * FROM Users LIMIT %s OFFSET %s"
        cursor.execute(query, (per_page, offset))
        players = cursor.fetchall()
        
        return jsonify({
            'success': True,
            'players': players,
            'pagination': {
                'page': page,
                'per_page': per_page,
                'total': total,
                'pages': (total + per_page - 1) // per_page
            }
        })
        
    except Error as e:
        return jsonify({'error': f'Database error: {str(e)}'}), 500
    finally:
        if connection.is_connected():
            cursor.close()
            connection.close()

# Not using this yet, but would be fun 
@app.route('/api/highscore', methods=['GET'])
@require_api_key
def get_highscore():
    """Get the top 10 players ranked by level and xp"""
    connection = get_db_connection()
    if not connection:
        return jsonify({'error': 'Database connection failed'}), 500

    try:
        cursor = connection.cursor(dictionary=True)
        query = """
            SELECT user_id, name, lvl, xp, gold
            FROM Users
            ORDER BY lvl DESC, xp DESC
            LIMIT 10
        """
        cursor.execute(query)
        highscores = cursor.fetchall()

        return jsonify({
            'success': True,
            'highscores': highscores
        })

    except Error as e:
        return jsonify({'error': f'Database error: {str(e)}'}), 500
    finally:
        if connection.is_connected():
            cursor.close()
            connection.close()



@app.errorhandler(404)
def not_found(error):
    return jsonify({'error': 'Endpoint not found'}), 404

@app.errorhandler(500)
def internal_error(error):
    return jsonify({'error': 'Internal server error'}), 500

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)
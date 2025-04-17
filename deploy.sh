#!/bin/bash

# Colors for output
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo -e "${YELLOW}Starting deployment process...${NC}"

# Check if git is installed
if ! command -v git &> /dev/null; then
    echo "Git is not installed. Please install git first."
    exit 1
fi

# Check if we're in a git repository
if [ ! -d .git ]; then
    echo "Initializing git repository..."
    git init
    git add .
    git commit -m "Initial commit"
fi

# Create .gitignore if it doesn't exist
if [ ! -f .gitignore ]; then
    echo "Creating .gitignore file..."
    cat > .gitignore << EOL
__pycache__/
*.pyc
.DS_Store
.env
node_modules/
.vscode/
*.log
EOL
fi

# Add all files
echo "Adding files to git..."
git add .

# Commit changes
echo "Committing changes..."
git commit -m "Daily deployment $(date +%Y-%m-%d)"

# Push to remote repository
echo "Pushing to remote repository..."
git push origin main

echo -e "${GREEN}Deployment completed successfully!${NC}"
echo -e "${YELLOW}Next deployment will be at 00:00 UTC${NC}" 
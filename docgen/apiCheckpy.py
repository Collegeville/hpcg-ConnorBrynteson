"""

import requests

# Replace these variables with the appropriate values
repo_owner = "hpcg-benchmark"
repo_name = "hpcg"

# GitHub API URL for the repository commits
commits_url = f"https://api.github.com/repos/{repo_owner}/{repo_name}/commits"

# Function to get the latest commit
def get_latest_commit():
    response = requests.get(commits_url)
    if response.status_code == 200:
        commits = response.json()
        if commits:
            return commits[2]  # Return the latest commit
    return None

# Function to get the code changes for a specific commit
def get_commit_changes(commit_url):
    response = requests.get(commit_url)
    if response.status_code == 200:
        commit_data = response.json()
        files = commit_data.get("files", [])
        return files
    return None

def main():
    # Get the latest commit
    latest_commit = get_latest_commit()
    if latest_commit:
        commit_url = latest_commit["url"]
        print(f"Latest commit URL: {commit_url}")
        print(f"Commit message: {latest_commit['commit']['message']}")
        
        # Get the changes in the latest commit
        changes = get_commit_changes(commit_url)
        if changes:
            for file in changes:
                print(f"File: {file['filename']}")
                print("Changes:")
                print(file['patch'])
        else:
            print("No changes found for the latest commit.")
    else:
        print("No commits found in the repository.")

if __name__ == "__main__":
    main()


"""

#Testing by date

import requests
from datetime import datetime, timedelta

# Replace these variables with the appropriate values
repo_owner = "hpcg-benchmark"
repo_name = "hpcg"

# GitHub API URL for the repository commits
commits_url = f"https://api.github.com/repos/{repo_owner}/{repo_name}/commits"

# Function to get commits
def get_commits():
    response = requests.get(commits_url)
    if response.status_code == 200:
        return response.json()
    return []

# Function to parse the date string to datetime object
def parse_date(date_str):
    return datetime.strptime(date_str, "%Y-%m-%dT%H:%M:%SZ")

# Function to find the closest commits to a given date
def find_closest_commits(target_date, commits):
    target_date = parse_date(target_date)
    closest_commits = []
    min_diff = timedelta.max

    for commit in commits:
        commit_date = parse_date(commit["commit"]["committer"]["date"])
        date_diff = abs(commit_date - target_date)

        if date_diff < min_diff:
            min_diff = date_diff
            closest_commits = [commit]
        elif date_diff == min_diff:
            closest_commits.append(commit)

    return closest_commits

# Function to get the code changes for a specific commit
def get_commit_changes(commit_url):
    response = requests.get(commit_url)
    if response.status_code == 200:
        commit_data = response.json()
        files = commit_data.get("files", [])
        return files
    return None

def main():
    target_date = "2023-06-01T00:00:00Z"  # Replace with your target date in ISO format (YYYY-MM-DDTHH:MM:SSZ)

    # Get all commits
    commits = get_commits()
    if not commits:
        print("No commits found in the repository.")
        return

    # Find the closest commits to the target date
    closest_commits = find_closest_commits(target_date, commits)
    if not closest_commits:
        print("No closest commits found.")
        return

    for commit in closest_commits:
        commit_url = commit["url"]
        print(f"Commit URL: {commit_url}")
        print(f"Commit message: {commit['commit']['message']}")
        print(f"Commit date: {commit['commit']['committer']['date']}")

        # Get the changes in the commit
        changes = get_commit_changes(commit_url)
        if changes:
            for file in changes:
                print(f"File: {file['filename']}")
                print("Changes:")
                print(file['patch'])
        else:
            print("No changes found for this commit.")
        print("\n")

if __name__ == "__main__":
    main()

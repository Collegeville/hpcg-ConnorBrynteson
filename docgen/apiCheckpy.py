import requests
from transformers import pipeline, AutoTokenizer

# Replace these variables with the appropriate values
verison = 17.4
repo_owner = "github-tools"
repo_name = "github-release-notes"
token = ""

# GitHub API URL for the repository commits
commits_url = f"https://api.github.com/repos/{repo_owner}/{repo_name}/commits"

# Function to get all commits with SHA and dates
def get_all_commits():
    commits = []
    headers = {
        'Authorization': f'token {token}'
    }
    params = {
        'per_page': 100  # Number of commits to return per page (maximum is 100)
    }
    response = requests.get(commits_url, headers=headers, params=params)
    while response.status_code == 200 and response.json():
        data = response.json()
        for commit in data:
            commits.append({
                'sha': commit['sha'],
                'date': commit['commit']['author']['date'],
                'message': commit['commit']['message']
            })
        # Check if there is a next page of results
        if 'next' in response.links:
            response = requests.get(response.links['next']['url'], headers=headers, params=params)
        else:
            break
    return commits

# Function to filter commits between two given SHAs
def get_commits_between(start_sha, end_sha):
    all_commits = get_all_commits()
    start_index = next((i for i, commit in enumerate(all_commits) if commit['sha'] == start_sha), None)
    end_index = next((i for i, commit in enumerate(all_commits) if commit['sha'] == end_sha), None)

    if start_index is None or end_index is None:
        raise ValueError("Start SHA or End SHA not found in the repository commits")

    if start_index < end_index:
        raise ValueError("Start SHA must be an earlier commit than End SHA")

    return all_commits[end_index:start_index+1]

def commit_by_sha(sha, token=None):
    commit_url = f"{commits_url}/{sha}"
    headers = {}
    if token:
        headers['Authorization'] = f'token {token}'

    response = requests.get(commit_url, headers=headers)
    if response.status_code == 200:
        return response.json()
    else:
        print(f"Failed to fetch commit {sha}: {response.status_code}, {response.text}")
        return None
        

def get_commit_by_sha():
    sha = input("Enter the SHA of the commit: ")
    commit_details = get_commit_by_sha(sha, token=None)
    if commit_details:
        print(f"Commit SHA: {commit_details['sha']}")
        print(f"Author: {commit_details['commit']['author']['name']}")
        print(f"Date: {commit_details['commit']['author']['date']}")
        print(f"Message: {commit_details['commit']['message']}")
        print("Files changed:")
        for file in commit_details.get("files", []):
            print(f"File: {file['filename']}")
            print("Changes:")
            print(file['patch'])
    else:
        print("No details found for the given SHA.")
        
    return commit_details

summarizer = pipeline("summarization", model="facebook/bart-large-cnn")
tokenizer = AutoTokenizer.from_pretrained("facebook/bart-large-cnn")


def summarize_code(code):
    if not code.strip():
        return "The code is empty"
    # Prepare the input text
    tokens = tokenizer.tokenize(code)
    
    if len(tokens) > 144:
        summary = summarizer(code, max_length = 144, min_length = 0, do_sample=False)[0]['summary_text']
    else:
        summary = summarizer(code, max_length = len(tokens) + 100, min_length = 0, do_sample=False)[0]['summary_text']

    return summary

def get_all_message_summary():
    commits = get_all_commits()
    for commit in commits:
        commit_message = commit['message'].replace('\n', ' ')
        commit_message = commit_message.replace('\r', ' ')
        commit_message = commit_message.replace('\t', ' ')
        commit_message = commit_message.replace('  ', ' ')
        if(commit_message.startswith("Merge") or commit_message.startswith("Merge pull request")):
            continue
        summary = summarize_code(commit_message)
        print(f"Summary: {summary}")

path= "docgen\output_test.md"

"""
def get_messages_by_sha(sha1, sha2):
    commits = get_commits_between(sha1, sha2)
    with open(path, 'w') as f:
        for commit in commits:
            commit_message = commit['message'].replace('\n', ' ')
            commit_message = commit_message.replace('\r', ' ')
            commit_message = commit_message.replace('\t', ' ')
            commit_message = commit_message.replace('  ', ' ')
            if(commit_message.startswith("Merge") or commit_message.startswith("Merge pull request") or "revert" in commit_message.lower()):
                continue
            summary = summarize_code(commit_message)
            if "CNN" in summary:
                continue
            f.write(f"Summary: {summary}\n" + commit['sha'] + "\n")
"""

def release_notes(sha1, sha2, Version):
    commits = get_commits_between(sha1, sha2)
    
    with open(path, 'w') as f:
        f.write(f"Release Notes Version {Version}\n\n")
        
        for commit in commits:
            commit_message = commit['message'].replace('\n', ' ').replace('\r', ' ').replace('\t', ' ').replace('  ', ' ')
            
            if(commit_message.startswith("Merge") or commit_message.startswith("Merge pull request")):
                continue
            
            if("fix" in commit_message.lower() or "add" in commit_message.lower() or "remove" in commit_message.lower() or "update" in commit_message.lower() or "return" in commit_message.lower()):
                summary = summarize_code(commit_message)
                
                if "CNN" in summary:
                    continue
                url = f"https://github.com/{repo_owner}/{repo_name}/commit/"
                f.write(f"Commit Summary: {summary}\n" + url+commit['sha'] + "\n\n")
    

# Main function to call the above functions

def main():
    #print("Code summary:")
    #get_all_message_summary() 
    
    #print("Commits between two SHAs:")
    #get_messages_by_sha('5429f72eedd269303cadc7394054e69b442ed7cc', '114602d458d1034faa52b71e4c15aba9b3a17698')
    
    release_notes("62c9289306d4b9abd7170d0fbdf29b25a4dd112c", "ff0ddae3938db3cfcca5ef317d74eee034c32cc5", verison)


if __name__ == "__main__":
    main()

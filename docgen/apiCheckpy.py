import requests
from transformers import pipeline, AutoTokenizer

# Replace these variables with the appropriate values
verison = 0.0
repo_owner = ""
repo_name = ""
token = ""
#This is the list of words used in the commit message to determine if it is a feature or a bug fix
keywords = ["fix", "add", "remove", "update", "return"]

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

# Function could be removed or made to look for something like the version so you dont have to add it but I think that would be more work than just adding the version

def get_all_message_summary():
    commits = get_all_commits()
    for commit in commits:
        commit_message = ' '.join(commit['message'].split())
        
        if(commit_message.startswith("Merge") or commit_message.startswith("Merge pull request")):
            continue
        summary = summarize_code(commit_message)
        print(f"Summary: {summary}")

path= "docgen\output_test.md"

def release_notes(sha1, sha2, Version):
    commits = get_commits_between(sha1, sha2)
    
    with open(path, 'w') as f:
        f.write(f"Release Notes Version {Version}\n\n")
        
        for commit in commits:
            commit_message = ' '.join(commit['message'].split())
            
            if(commit_message.startswith("Merge") or commit_message.startswith("Merge pull request")):
                continue
            
            if any(keyword in commit_message for keyword in keywords):
                summary = summarize_code(commit_message)
                # I need to make it do a better job of filtering out and summarizing the commits because it is not doing a good job that is where we get CNN
                if "CNN" in summary:
                    continue
                url = f"https://github.com/{repo_owner}/{repo_name}/commit/"
                f.write(f"Commit Summary: {summary}\n" + url+commit['sha'] + "\n\n")


if __name__ == "__main__":
    release_notes("", "", verison)

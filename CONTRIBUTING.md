# Team contribution workflow

The course uses commit history to assess individual contributions. Each member works under their own GitHub account, verifies their assigned changes and opens a pull request into dev. The initial project setup provides the common starting point. Individual contributions are recorded through subsequent commits, reviews and measurements.

- [Khoa - scheduler, software timer and Automatic mode](docs/team/khoa.md)
- [Minh - board, buttons, Manual mode and mode switching](docs/team/minh.md)
- [Long - DHT20, LCD and Timing Adjustment](docs/team/long.md)

Accept the repository invitation before pushing. The owner can manage invitations under **Settings > Collaborators**. Members may also create a fork and submit a pull request from a branch on their fork.

Use an email verified on your GitHub account, or the exact noreply address shown in **Settings > Emails**, so commits are attributed to the correct account.

```bash
git clone https://github.com/khoatruong2805/co3009-two-road-traffic-light.git
cd co3009-two-road-traffic-light
git switch dev
git pull --ff-only
git switch -c feature/scheduler-boundaries
git config --local user.name "Your name"
git config --local user.email "Your verified GitHub email"
# Make and verify changes, then review the staged diff.
git add -p
# Stage new files explicitly with: git add path/to/file
git diff --cached
git commit -m "scheduler: verify cancellation at equal deadlines"
git push -u origin HEAD
```

Describe the behavior changed, reason, verification results and remaining limitations in the pull request. Another member reviews the work. Use **Create a merge commit** to preserve the individual commits.

Before submission, inspect `git shortlog -sne dev` and check that each member's commits link to the intended GitHub account. Merge approved work into main, create a submission tag such as v1.0-final, and record the tag and `git rev-parse HEAD` on the report cover.
